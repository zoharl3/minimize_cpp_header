
#pragma once

struct ast_visitor : public RecursiveASTVisitor<ast_visitor> {
    ast_visitor( vector<Rewriter> &writers_, ASTContext &ctx_, string &fname_ );

    bool VisitClassTemplateDecl( ClassTemplateDecl *d );
    bool VisitCXXRecordDecl( CXXRecordDecl *d );
    bool VisitFriendDecl( FriendDecl *d );
    bool VisitFunctionDecl( FunctionDecl *d ); // declared in class ASTNodeTraverser
    bool visit_function_decl( FunctionDecl *d );
    bool VisitFunctionTemplateDecl( FunctionTemplateDecl *d );

    string to_str( const SourceRange &range );
    string to_str( const SourceLocation &begin, const SourceLocation &end );
    string to_str( Decl *d );
    SourceRange srange( const SourceRange &partial_range );
    SourceRange srange( Decl *d );
    SourceLocation get_loc_for_end_of_token( const SourceLocation &loc );
    SourceLocation find_loc( const SourceLocation &begin, char ch );
    pair<string, string> get_templ_param_string( RedeclarableTemplateDecl *d );
    void instantiate_func( string parent_name, string func_name, vector<ParmVarDecl *> &param_decls );

    vector<Rewriter> &writers;
    ASTContext &ctx;
    SourceManager &sm;

    CXXRecordDecl *m_tmpl_class_decl;
    string m_class_tmpl, m_class_tmpl_init;
    bool m_b_friend, m_b_tmpl_func;
    int m_param_counter;
    string class_inst, func_inst;
};
