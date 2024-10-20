
// Clang requires from the file extension to be c++ to detect it as such, e.g., .hh, .hpp, .cpp.

#include "stdafx.h"

#include "visitor.h"

struct ast_consumer : public ASTConsumer {
    ast_consumer( vector<Rewriter> &rewriters, ASTContext &ctx, string &fname ) : visitor( rewriters, ctx, fname ) {}

    virtual bool HandleTopLevelDecl( DeclGroupRef group_ref ) {
        if ( visitor.ctx.getDiagnostics().getNumErrors() > 0 ) {
            println( "there are compilation errors" );
            return false;
        }

        for ( DeclGroupRef::iterator b = group_ref.begin(), e = group_ref.end(); b != e; ++b )
            visitor.TraverseDecl( *b );
        return true;
    }

    ast_visitor visitor;
};

vector<string> parse( int argc, const char **argv ) {
    vector<string> ret;

    vector<char const *> commandLine;
    commandLine.push_back( "" );

    for ( int i = 1; i < argc; ++i )
        commandLine.push_back( argv[i] );

    const shared_ptr<CompilerInvocation> comp( createInvocation( ArrayRef( commandLine ) ) );
    if ( !comp )
        return vector<string>();

    LangOptions &lo = comp->getLangOpts();
    lo.DelayedTemplateParsing = 0;
    lo.CPlusPlus17 = 1;
    lo.Bool = 1;

    auto &ho = comp->getHeaderSearchOpts();

    const auto pchContainerOps = make_shared<PCHContainerOperations>();
    const IntrusiveRefCntPtr<DiagnosticsEngine> diagnosticsEngine( CompilerInstance::createDiagnostics( new DiagnosticOptions ) );

    const unique_ptr<ASTUnit> ast( ASTUnit::LoadFromCompilerInvocationAction( comp, pchContainerOps, diagnosticsEngine ) );

    if ( ast == nullptr || diagnosticsEngine->getNumErrors() > 0 )
        return vector<string>();

    ASTContext &ctx = ast->getASTContext();
    auto &sm = ctx.getSourceManager();
    string fname;
    fname = sm.getFilename( sm.getLocForStartOfFile( sm.getMainFileID() ) );

    vector<Rewriter> rewriters( 2, Rewriter( ctx.getSourceManager(), ctx.getLangOpts() ) );

    ast_visitor visitor( rewriters, ctx, fname );
    visitor.TraverseDecl( ctx.getTranslationUnitDecl() );

    ret.push_back( visitor.class_inst );
    ret.push_back( visitor.func_inst );

    string code;
    for ( int i = 0; i < rewriters.size(); ++i ) {
        const RewriteBuffer *rewrite_buffer = rewriters[i].getRewriteBufferFor( ctx.getSourceManager().getMainFileID() );
        if ( rewrite_buffer )
            code = string( rewrite_buffer->begin(), rewrite_buffer->end() );
        ret.push_back( code );
    }

    //ctx.getTranslationUnitDecl()->dumpColor();

    ret.push_back( fname );

    return ret;
}

int main( int argc, const char **argv ) {
    auto vec = parse( argc, argv );
    if ( vec.empty() )
        return 1;

    array<string, 5> arr;
    for ( int i = 0; i < arr.size(); ++i )
        arr[i] = vec[i];
    auto [class_inst, func_inst, code, code2, fname] = arr;

    println();

    fs::path path( fname );
    string basename = ( path.parent_path() / path.stem() ).generic_string();

    if ( code.empty() )
        warn( "no changes" );

    ofstream fcode( basename + "_min_lean.hpp", ios_base::binary );
    fcode << "\ntypedef bool _Bool;\n\n";
    fcode << code;

    ofstream fcode2( basename + "_min.hpp", ios_base::binary );
    fcode2 << code2;

    ofstream fIns( basename + "_min_ins.hpp", ios::binary );
    fIns << class_inst;
    fIns << func_inst + "\n\n";

    //pause();

    return 0;
}
