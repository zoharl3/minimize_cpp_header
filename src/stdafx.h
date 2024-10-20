
// precompiled header
#pragma once

// warning C4267: 'initializing': conversion from 'size_t' to 'int', possible loss of data
#pragma warning( disable : 4267 )

// for assert(); needs to be before related includes
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <map>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TargetParser/Host.h"
#include "clang/Parse/ParseAST.h"

#include "logging.h"

using namespace std;
namespace fs = std::filesystem;

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::driver;
using namespace clang::tooling;

static auto &lout = llvm::outs();
