
#include "stdafx.h" 

#include "visitor.h"

// a cleaner way?
bool is_template_type( const QualType &type ) {
    bool has_tmpl = type->isTemplateTypeParmType();
    string name = type.getAsString();

    if ( !has_tmpl ) {
        // has template expression
        has_tmpl |= name.find( '<' ) != string::npos;

        if ( has_tmpl ) {
            // specialized; if it's inside complex<>, then the specialized will refer to the complex
            auto type2 = type.getNonReferenceType()->getAs<TemplateSpecializationType>();
            if ( 0&& type2 != nullptr )
                has_tmpl = false;
        }
    }

    return has_tmpl;
}

// like the private Lexer::isObviouslySimpleCharacter()
bool isObviouslySimpleCharacter( char C ) {
    return C != '?' && C != '\\';
}

////// ast_visitor
string ast_visitor::to_str( const SourceRange &range ) {
    return string( sm.getCharacterData( range.getBegin() ), sm.getCharacterData( range.getEnd() ) );
}

string ast_visitor::to_str( const SourceLocation &begin, const SourceLocation &end ) {
    return string( sm.getCharacterData( begin ), sm.getCharacterData( end ) );
}

string ast_visitor::to_str( Decl *d ) {
    return to_str( srange( d ) );
}

SourceLocation ast_visitor::get_loc_for_end_of_token( const SourceLocation &loc ) {
    return Lexer::getLocForEndOfToken( loc, 0, sm, LangOptions() );
}

// returns full source range
// https://stackoverflow.com/questions/79100005/source-range-of-vardecl
SourceRange ast_visitor::srange( const SourceRange &partial_range ) {
    auto end_loc = get_loc_for_end_of_token( partial_range.getEnd() );
    auto full_range = SourceRange( partial_range.getBegin(), end_loc );
    return full_range;
}

SourceRange ast_visitor::srange( Decl *d ) {
    return srange( d->getSourceRange() );
}

// the lexer functions work only within a token range
// similar to Lexer::getTokenPrefixLength()
// need to implement this with iterating token; this code doesn't support macro expansion
SourceLocation ast_visitor::find_loc( const SourceLocation &start, char ch ) {
    bool invalid = false;
    const char *s = sm.getCharacterData( start, &invalid );
    int len = 0;
    if ( invalid || isObviouslySimpleCharacter( *s ) )
        len = 0;
    else {
        while ( s != nullptr && *s != ch )
            ++s;
    }
    return start.getLocWithOffset( len );
}

// reconstruct param string
pair<string, string> ast_visitor::get_templ_param_string( RedeclarableTemplateDecl *d ) {
    string param, param_init;
    for ( auto p : *d->getTemplateParameters() ) {
        if ( !param_init.empty() ) {
            param_init += ", ";
            param += ", ";
        }

        // how to get type? what if it's more complicated (e.g., enable_if)?
        // auto p_s = ? + " " + p->getNameAsString();
        auto p_s = to_str( srange( p->getSourceRange() ) );

        // a cleaner way?
        int pos = p_s.find( '=' );
        auto p_s2 = pos >= 0 ? p_s.substr( 0, pos ) : p_s;

        param_init += p_s;
        param += p_s2;
    }

    return make_pair( param, param_init );
}

ast_visitor::ast_visitor( vector<Rewriter> &writers_, ASTContext &ctx_, string &fname_ ) : writers( writers_ ), ctx( ctx_ ), sm( ctx.getSourceManager() ) {
    m_b_tmpl_func = m_b_friend = false;
    m_tmpl_class_decl = nullptr;
    m_param_counter = 0;
}

// a CXXRecordDecl must follow
bool ast_visitor::VisitClassTemplateDecl( ClassTemplateDecl *d ) {
    if ( !( sm.isInMainFile( d->getLocation() ) ) )
        return true;

    auto name = d->getNameAsString();
    emphasize( "- VisitClassTemplateDecl( %s )", name.c_str() );

    m_tmpl_class_decl = d->getTemplatedDecl(); // the class

    m_class_tmpl_init = to_str( d );

    auto [param, param_init] = get_templ_param_string( d );

    m_class_tmpl = format( "template <{}>", param );

    return true;
}

bool ast_visitor::VisitCXXRecordDecl( CXXRecordDecl *d ) {
    if ( !( sm.isInMainFile( d->getLocation() ) ) )
        return true;

    emphasize( "- VisitCXXRecordDecl()" );

    //d->dumpColor();

    bool is_tmpl = m_tmpl_class_decl == d;

    if ( !is_tmpl )
        m_class_tmpl = "";

    if ( is_tmpl && d->getBraceRange().isValid() ) {
        auto start = d->getSourceRange().getBegin();
        auto tok_end_loc = get_loc_for_end_of_token( start );
        auto struct_s = to_str( start, tok_end_loc );
        auto name = d->getTypeForDecl()->getCanonicalTypeInternal().getAsString();

        // instantiate class
        m_tmpl_inst += "template\n";
        m_tmpl_inst += format( "{} {};\n\n", struct_s, name );
    }

    println();

    return true;
}

bool ast_visitor::VisitTypeAliasDecl( TypeAliasDecl *d ) {
    if ( !( sm.isInMainFile( d->getLocation() ) ) )
        return true;

    if ( !d->isCXXClassMember() )
        return true;

    emphasize( "- TypeAliasDecl()" );

    m_func_inst += "    " + to_str( d ) + ";\n\n";

    return true;
}

bool ast_visitor::VisitVarDecl( VarDecl *d ) {
    if ( !( sm.isInMainFile( d->getLocation() ) ) )
        return true;

    if ( !d->isCXXClassMember() )
        return true;

    if ( !d->isConstexpr() )
        return true;

    emphasize( "- VisitVarDecl()" );

    m_func_inst += "    " + to_str( d ) + ";\n\n";

    return true;
}

// a FunctionDecl must follow
bool ast_visitor::VisitFunctionTemplateDecl( FunctionTemplateDecl *d ) {
    if ( !( sm.isInMainFile( d->getLocation() ) ) )
        return true;

    emphasize( "- VisitFunctionTemplateDecl()" );

    //auto [param, param_init] = get_templ_param_string( d );

    m_b_tmpl_func = true;

    return true;
}

// a FunctionDecl must follow
bool ast_visitor::VisitFriendDecl( FriendDecl *d ) {
    if ( !( sm.isInMainFile( d->getLocation() ) ) )
        return true;

    emphasize( "- VisitFriendDecl()" );

    m_b_friend = true;

    return true;
}

bool ast_visitor::VisitFunctionDecl( FunctionDecl *d ) {
    if ( !( sm.isInMainFile( d->getLocation() ) ) )
        return true;

    bool ret = visit_function_decl( d );

    // reset
    m_b_tmpl_func = m_b_friend = false;

    println();

    return ret;
}

bool ast_visitor::visit_function_decl( FunctionDecl * d ) {
    DeclContext *parent = d->getParent();
    CXXRecordDecl *parent_class = isa<CXXRecordDecl>( parent ) ? (CXXRecordDecl *)parent : nullptr;
    string parent_class_name;
    if ( parent_class )
        parent_class_name = m_class_tmpl.empty() ? parent_class->getNameAsString() : parent_class->getTypeForDecl()->getCanonicalTypeInternal().getAsString();

    auto name_L_loc = d->getNameInfo().getBeginLoc();
    auto name_R_loc = srange( d->getNameInfo().getSourceRange() ).getEnd();
    auto name = to_str( name_L_loc, name_R_loc );
    string qname = name;
    if ( parent_class )
        qname = parent_class_name + "::" + qname;

    if ( !d->hasBody() ) {
        if ( !parent_class ) {
            // A global function without a body means forward declaration.
            // It may be a friend in a class, but it won't have an effect on the logic.
            println( format( "- forward declaration of {}: {}", name, d->getType().getAsString() ) );
        }

    } else {
        emphasize( format( "- VisitFunctionDecl( {} )", qname ) );

        srange( d ).print( lout, sm );
        println();
        //d->dumpColor();

        Stmt *body_stmt = d->getBody();
        if ( !body_stmt ) {
            warn( "body is null (due to optimization?)" );
            return true;
        }

        // graph
        if ( 0 )
            body_stmt->viewAST(); // in debug build only

        // skip
        if ( 0&& name == "complicated_func" ) {
            println( "skipping" );
            return true;
        }

        // ctor
        bool b_ctor = isa<CXXConstructorDecl>( d ) || isa<CXXDestructorDecl>( d );
        CXXConstructorDecl *ctor = isa<CXXConstructorDecl>( d ) ? (CXXConstructorDecl *)d : nullptr;

        bool b_param = d->getNumParams() > 0;

        // parts
        SourceLocation L_brace_loc;
        L_brace_loc = body_stmt->getBeginLoc();
        SourceLocation param_R_loc;
        if ( b_param )
            param_R_loc = srange( d->getParametersSourceRange() ).getEnd();

        string ret_type;
        if ( !b_ctor ) {
            auto ret_range = d->getReturnTypeSourceRange();
            if ( ret_range.isValid() ) {
                auto type = d->getReturnType();
                ret_type = type.getAsString();
                if ( d->isStatic() )
                    ret_type = "static " + ret_type;
            } else
                warn( "the return type source range is invalid" );
        }

        string param_R_to_L_brace;
        if ( b_param )
            param_R_to_L_brace = to_str( param_R_loc, L_brace_loc );
        string name_L_to_L_brace = to_str( name_L_loc, L_brace_loc );
        string name_R_to_L_brace = to_str( name_R_loc, L_brace_loc );

        string param_s, param_init_s;
        vector<ParmVarDecl*> param_decls;
        if ( d->getNumParams() ) {
            // reconstruct param string
            for ( auto p : d->parameters() ) {
                if ( !param_init_s.empty() ) {
                    param_init_s += ", ";
                    param_s += ", ";
                }

                param_decls.push_back( p );

                auto type = p->getType();
                string type_s = type.getAsString();
                auto p_s = type_s + " " + p->getNameAsString();

                param_init_s += p_s;
                param_s += p_s;

                if ( p->hasInit() ) {
                    auto init = to_str( srange( p->getInit()->getSourceRange() ) );
                    param_init_s += " = " + init;
                }
            }
        }

        // delete def
        writers[0].RemoveText( srange( d ) );

        // write header
        string proto;
        if ( m_b_friend )
            proto += "friend ";
        if ( !ret_type.empty() )
            proto += ret_type + " ";
        if ( b_param ) {
            proto += format( "{}( {}", name, param_s );
            if ( ctor )
                proto += " )";
            else
                proto += param_R_to_L_brace;
        } else if ( ctor )
            proto += format( "{}()", name );
        else
            proto += name_L_to_L_brace;
        boost::trim_right( proto );
        proto += ";";

        writers[0].InsertText( d->getEndLoc().getLocWithOffset( 1 ), proto, true, true );

        // export nontemplate
        if ( !b_ctor && !m_b_tmpl_func )
            writers[1].InsertText( d->getBeginLoc(), "MIN_HEADER_EXPORT ", true, true );

        // instantiate
        if ( m_b_tmpl_func ) {
            string proto;
            proto += ret_type + " ";
            if ( b_param ) {
                proto += format( "{}( {}", qname, param_s );
                if ( ctor )
                    proto += " )";
                else
                    proto += param_R_to_L_brace;
            } else
                proto += name_L_to_L_brace;
            boost::trim_right( proto );

            m_tmpl_inst += "template\n" + proto + ";\n\n";
        } else if ( !b_ctor ) {
            // force usage
            instantiate_func( parent_class_name, name, param_decls );
        }
    }

    return true;
}

// instantiation by usage
void ast_visitor::instantiate_func( string parent_name, string func_name, vector<ParmVarDecl *> &param_decls ) {
    string param_s;
    for ( auto &p : param_decls ) {
        auto type = p->getType().getNonReferenceType();
        string type_s = type.getAsString();

        string pname = format( "var{}", m_param_counter++ );

        auto decl_s = format( "    {} *{} = nullptr;\n", type_s, pname ); // avoid "uninitialized local variable" warning
        m_func_inst += decl_s;

        if ( !param_s.empty() )
            param_s += ", ";
        param_s += format( "*{}", pname );
    }

    if ( parent_name.empty() )
        m_func_inst += format( "    {}( {} );\n\n", func_name, param_s );

    else {
        string vname = format( "cls_var{}", m_param_counter++ );
        m_func_inst += format( "    {} *{} = nullptr;\n", parent_name, vname );
        m_func_inst += format( "    {}->{}( {} );\n\n", vname, func_name, param_s );
    }
}
