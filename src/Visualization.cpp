/*
 **************** Visualization.cpp *****************
 * This file implements visualization methods of	*
 * the node classes in the AST.						*
 ****************************************************
 */

#include "AST.hpp"

using namespace std;
using namespace AST;

//去除转义字符
string getString(string name){
	int pos = 0;
	int len = name.length();
	string res;
	while (name[pos] != '\n' && name[pos] != '\0')
	{
		++pos;
	}
	if (pos == len)
		return name;
	string s1 = getString(name.substr(0, pos));
	string s2 = getString(name.substr(pos + 1, len));
	res = s1 + "\\" + "n" + s2;
	return res;
}

string getJson(string name) {
	// int pos = 0;
	// while(name[pos] != '\0')
	// {
	//	 if(name[pos] == '\n')
	//		 return "{ \"name\" : \"" + getString(name) + "\" }";
	// }
	return "{ \"name\" : \"" + getString(name) + "\" }";
}

string getJson(char c) {
	string name(1, c);
	// int pos = 0;
	// while(name[pos] != '\0')
	// {
	//	 if(name[pos] == '\n')
	//		 return "{ \"name\" : \"" + getString(name) + "\" }";
	// }
	return "{ \"name\" : \"" + name + "\" }";
}

string getJson(string name, vector<string> children) {
	string result = "{ \"name\" : \"" + name + "\", \"children\" : [ ";
	int i = 0;
	for (auto& child : children) {
		if (i != children.size() - 1)
			result += child + ", ";
		else
			result += child + " ";
		i++;
	}
	return result + " ] }";
}

string getJson(string name, string value) {
	return getJson(name, vector<string>{value});
}

string getJson(string name, string value, vector<string> children) {
	string result = "{ \"name\" : \"" + name + "\", \"value\" : \"" + value + "\", \"children\" : [ ";
	int i = 0;
	for (auto& child : children) {
		if (i != children.size() - 1)
			result += child + ", ";
		else
			result += child + " ";
		i++;
	}
	return result + " ] }";
}

string Program::astJson() {
	vector<string> declJson;	//children node json
	for (auto x : *_Decls)
		declJson.push_back(x->astJson());
	// children.push_back(getJson("Decls", declJson));
	return getJson("Program", declJson);
	//return getJson("program",vector<string>{Decl->getJson()});
}

string FuncDecl::astJson() {
	vector<string> children;
	children.push_back(getJson("Return Type", _RetType->astJson()));
	children.push_back(getJson("Function Name", getJson(_Name)));
	vector<string> argListJson;
	for (auto& x : *_ArgList)
		argListJson.push_back(x->astJson());
	if (_ArgList->_VarArg == true)
		argListJson.push_back(getJson("...", ""));
	children.push_back(getJson("ArgList", argListJson));
	if (_FuncBody != NULL)
		children.push_back(_FuncBody->astJson());
	return getJson("FuncDeclaration", children);
}

string ArgList::astJson() {
	if (_VarArg == true)
		return getJson("..", getJson("True"));
	else if (_VarArg == false)
		return getJson("..", getJson("False"));
}

string FuncBody::astJson() {
	vector<string> funcBodyJson;	//children node json
	for (auto& x : *_Content)
		funcBodyJson.push_back(x->astJson());
	// children.push_back(getJson("Decls", declJson));
	return getJson("FuncBody", funcBodyJson);
}

string VarDecl::astJson() {
	vector<string> children;
	children.push_back(_VarType->astJson());
	vector<string> varListJson;
	for (auto& x : *_VarList)
		varListJson.push_back(x->astJson());
	children.push_back(getJson("VarList", varListJson));
	return getJson("VarDeclaration", children);
}

string VarInit::astJson() {
	vector<string> children;
	children.push_back(getJson(_Name));
	if (_InitialExpr != NULL)
	{
		//children.push_back(getJson("="));
		children.push_back(_InitialExpr->astJson());
	}
	// if(_InitialConstant != NULL)
	// {
	// children.push_back(getJson("="));
	// children.push_back(getJson("Initial Constant",_InitialConstant->astJson()));
	// }
	//children.push_back(_InitialExpr->astJson());
	//children.push_back(_InitialConstant->astJson());
	return getJson("Varinit", children);
}

string TypeDecl::astJson() {
	vector<string> children;
	children.push_back(_VarType->astJson());
	children.push_back(getJson(_Alias));
	return getJson("TypeDeclaration", children);
	// return "";
}

string DefinedType::astJson() {
	vector<string> children;
	children.push_back(getJson(_Name));
	return getJson("Definedtype", children);
	// return "";
}

string PointerType::astJson() {
	vector<string> children;
	children.push_back(_BaseType->astJson());
	return getJson("PointerType", children);
	// return "";
}

string ArrayType::astJson() {
	vector<string> children;
	children.push_back(_BaseType->astJson());
	children.push_back(getJson(to_string(_Length)));
	return getJson("ArrayType", children);
	// return "";
}

string StructType::astJson() {
	vector<string> children;
	for (auto& x : *_StructBody)
		children.push_back(x->astJson());
	return getJson("StructType", children);
}

string FieldDecl::astJson() {
	vector<string> children;
	children.push_back(_VarType->astJson());
	vector<string> memListJson;
	for (auto x : *_MemList)
		memListJson.push_back(getJson(x));
	children.push_back(getJson("MemList", memListJson));
	return getJson("FieldDecl", children);
	// return "";
}

string EnumType::astJson() {
	vector<string> enumListJson;
	for (auto& x : *_EnmList)
		enumListJson.push_back(x->astJson());
	return getJson("EnumType", enumListJson);
	// return "";
}

string Enm::astJson() {
	vector<string> children;
	children.push_back(getJson(_Name));
	if (_hasValue == true)
		children.push_back(getJson(to_string(_Value)));
	return getJson("Enm", children);
	// return "";
}

string Arg::astJson()
{
	vector<string> children;
	children.push_back(_VarType->astJson());
	if (_Name != "") children.push_back(getJson(_Name));
	return getJson("Arg", children);
	// return "";
}

string Block::astJson() {
	vector<string> blockJson;	//children node json
	for (auto& x : *_Content)
		blockJson.push_back(x->astJson());
	// children.push_back(getJson("Decls", declJson));
	return getJson("Block", blockJson);
}

string IfStmt::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Condition->astJson());
	children.push_back(_Then->astJson());
	if (_Else != NULL)
		children.push_back(_Else->astJson());
	return getJson("IfStmt", children);
}

string WhileStmt::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Condition->astJson());
	if (_LoopBody != NULL)
		children.push_back(_LoopBody->astJson());
	return getJson("WhileStmt", children);
}

string DoStmt::astJson() {
	vector<string> children;	//children node json
	if (_LoopBody != NULL)
		children.push_back(_LoopBody->astJson());
	children.push_back(_Condition->astJson());
	return getJson("DoStmt", children);
}

string ForStmt::astJson() {
	vector<string> children;	//children node json
	if (_Initial != NULL)
		children.push_back(_Initial->astJson());
	if (_Condition != NULL)
		children.push_back(_Condition->astJson());
	if (_Tail != NULL)
		children.push_back(_Tail->astJson());
	if (_LoopBody != NULL)
		children.push_back(_LoopBody->astJson());
	return getJson("ForStmt", children);
}

string SwitchStmt::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Matcher->astJson());
	vector<string> caseListJson;
	for (auto x : *_CaseList)
		caseListJson.push_back(x->astJson());
	children.push_back(getJson("CaseList", caseListJson));
	return getJson("SwitchStmt", children);
}

string CaseStmt::astJson() {
	vector<string> children;	//children node json
	if (_Condition != NULL)
		children.push_back(_Condition->astJson());
	else
		children.push_back(getJson("Default"));
	vector<string> stmtListJson;
	for (auto x : *_Content)
		stmtListJson.push_back(x->astJson());
	children.push_back(getJson("StatmentList", stmtListJson));
	return getJson("CaseStmt", children);
}

string BreakStmt::astJson() {
	return getJson("BreakStmt");
}

string ContinueStmt::astJson() {
	return getJson("ContinueStmt");
}

string ReturnStmt::astJson() {
	vector<string> children;	//children node json
	if (_RetVal != NULL)
		children.push_back(_RetVal->astJson());
	return getJson("ReturnStmt", children);
}

string Subscript::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Array->astJson());
	children.push_back(_Index->astJson());
	return getJson("Subscript", children);
}

string SizeOf::astJson() {
	vector<string> children;	//children node json
	if (_Arg1 != NULL)
		children.push_back(_Arg1->astJson());
	if (_Arg2 != NULL)
		children.push_back(_Arg2->astJson());
	return getJson("Sizeof", children);
}

string FunctionCall::astJson() {
	vector<string> children;	//children node json
	children.push_back(getJson("Function Name", getJson(_FuncName)));
	vector<string> argListJson;
	for (auto x : *_ArgList)
		argListJson.push_back(x->astJson());
	children.push_back(getJson("ArgList", argListJson));
	return getJson("FunctionCall", children);
}

string StructReference::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Struct->astJson());
	children.push_back(getJson(_MemName));
	return getJson("StructReference", children);
}

string StructDereference::astJson() {
	vector<string> children;	//children node json
	children.push_back(_StructPtr->astJson());
	children.push_back(getJson(_MemName));
	return getJson("StructDereference", children);
}

string UnaryPlus::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("+"));
	children.push_back(_Operand->astJson());
	return getJson("UnaryPlus", children);
}

string UnaryMinus::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("-"));
	children.push_back(_Operand->astJson());
	return getJson("UnaryMinus", children);
}

string TypeCast::astJson() {
	vector<string> children;	//children node json
	children.push_back(_VarType->astJson());
	children.push_back(_Operand->astJson());
	return getJson("TypeCast", children);
}

string PrefixInc::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("++"));
	children.push_back(_Operand->astJson());
	return getJson("PrefixInc", children);
}

string PostfixInc::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Operand->astJson());
	//children.push_back(getJson("++"));
	return getJson("PostfixInc", children);
}

string PrefixDec::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("--"));
	children.push_back(_Operand->astJson());
	return getJson("PrefixDec", children);
}

string PostfixDec::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Operand->astJson());
	//children.push_back(getJson("--"));
	return getJson("PostfixDec", children);
}

string Indirection::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("*"));
	children.push_back(_Operand->astJson());
	return getJson("Indirection", children);
}

string AddressOf::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("&"));
	children.push_back(_Operand->astJson());
	return getJson("Addressof", children);
}

string LogicNot::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("!"));
	children.push_back(_Operand->astJson());
	return getJson("LogicNot", children);
}

string BitwiseNot::astJson() {
	vector<string> children;	//children node json
	//children.push_back(getJson("~"));
	children.push_back(_Operand->astJson());
	return getJson("BitwiseNot", children);
}

string Division::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("/"));
	children.push_back(_RHS->astJson());
	return getJson("Division", children);
}

string Multiplication::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("*"));
	children.push_back(_RHS->astJson());
	return getJson("Multiplication", children);
}

string Modulo::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("%"));
	children.push_back(_RHS->astJson());
	return getJson("Modulo", children);
}

string Addition::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("+"));
	children.push_back(_RHS->astJson());
	return getJson("Addition", children);
}

string Subtraction::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("-"));
	children.push_back(_RHS->astJson());
	return getJson("Subtraction", children);
}

string LeftShift::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("<<"));
	children.push_back(_RHS->astJson());
	return getJson("LeftShift", children);
}

string RightShift::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson(">>"));
	children.push_back(_RHS->astJson());
	return getJson("RightShift", children);
}

string LogicGT::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson(">"));
	children.push_back(_RHS->astJson());
	return getJson("LogicGT", children);
}

string LogicGE::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson(">="));
	children.push_back(_RHS->astJson());
	return getJson("LogicGE", children);
}

string LogicLT::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("<"));
	children.push_back(_RHS->astJson());
	return getJson("LogicLT", children);
}

string LogicLE::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("<="));
	children.push_back(_RHS->astJson());
	return getJson("LogicLE", children);
}

string LogicEQ::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("=="));
	children.push_back(_RHS->astJson());
	return getJson("LogicEQ", children);
}

string LogicNEQ::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("!="));
	children.push_back(_RHS->astJson());
	return getJson("LogicNEQ", children);
}

string BitwiseAND::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("&"));
	children.push_back(_RHS->astJson());
	return getJson("BitwiseAND", children);
}

string BitwiseXOR::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("^"));
	children.push_back(_RHS->astJson());
	return getJson("BitwiseXOR", children);
}

string BitwiseOR::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("|"));
	children.push_back(_RHS->astJson());
	return getJson("BitwiseOR", children);
}

string LogicAND::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("&&"));
	children.push_back(_RHS->astJson());
	return getJson("LogicAND", children);
}

string LogicOR::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("||"));
	children.push_back(_RHS->astJson());
	return getJson("LogicOR", children);
}

string TernaryCondition::astJson() {
	vector<string> children;	//children node json
	children.push_back(_Condition->astJson());
	children.push_back(_Then->astJson());
	children.push_back(_Else->astJson());
	return getJson("TernaryCondition", children);
}

string DirectAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("="));
	children.push_back(_RHS->astJson());
	return getJson("DirectAssign", children);
}

string DivAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("/="));
	children.push_back(_RHS->astJson());
	return getJson("DivAssign", children);
}

string MulAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("*="));
	children.push_back(_RHS->astJson());
	return getJson("MulAssign", children);
}

string ModAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("%="));
	children.push_back(_RHS->astJson());
	return getJson("ModAssign", children);
}

string AddAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("+="));
	children.push_back(_RHS->astJson());
	return getJson("AddAssign", children);
}

string SubAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("-="));
	children.push_back(_RHS->astJson());
	return getJson("SubAssign", children);
}

string SHLAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("<<="));
	children.push_back(_RHS->astJson());
	return getJson("SHLAssign", children);
}

string SHRAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson(">>="));
	children.push_back(_RHS->astJson());
	return getJson("SHRAssign", children);
}

string BitwiseANDAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("&="));
	children.push_back(_RHS->astJson());
	return getJson("BitwiseANDAssign", children);
}

string BitwiseXORAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("^="));
	children.push_back(_RHS->astJson());
	return getJson("BitwiseXORAssign", children);
}

string BitwiseORAssign::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson("|="));
	children.push_back(_RHS->astJson());
	return getJson("BitwiseORAssign", children);
}

string CommaExpr::astJson() {
	vector<string> children;	//children node json
	children.push_back(_LHS->astJson());
	//children.push_back(getJson(","));
	children.push_back(_RHS->astJson());
	return getJson("CommaExpr", children);
}

string Variable::astJson() {
	vector<string> children;	//children node json
	children.push_back(getJson(_Name));
	return getJson("Variable", children);
}

string GlobalString::astJson() {
	vector<string> children;	//children node json
	children.push_back(getJson(_Content));
	return getJson("Global String", children);
}

string Constant::astJson() {
	vector<string> children;	//children node json
	switch (_Type)
	{
	case BuiltInType::TypeID::_Bool:
		return (_Bool == true) ? getJson("Constant", getJson("True")) : getJson("Constant", getJson("False"));
		break;
	case BuiltInType::TypeID::_Short:
	case BuiltInType::TypeID::_Int:
	case BuiltInType::TypeID::_Long:
		return getJson("Constant", getJson(to_string(_Integer)));
		break;
	case BuiltInType::TypeID::_Char:
		//string s(1,_Character);
		return getJson("Constant", getJson(_Character));
		break;
	case BuiltInType::TypeID::_Float:
	case BuiltInType::TypeID::_Double:
		return getJson("Constant", getJson(to_string(_Real)));
		break;
	default:
		break;
	}
}

string BuiltInType::astJson() {
	switch (_Type)
	{
	case _Bool:
		return getJson("BuildInType", getJson("Bool"));
		break;
	case _Short:
		return getJson("BuildInType", getJson("Short"));
		break;
	case _Int:
		return getJson("BuildInType", getJson("Int"));
		break;
	case _Long:
		return getJson("BuildInType", getJson("Long"));
		break;
	case _Char:
		return getJson("BuildInType", getJson("Char"));
		break;
	case _Float:
		return getJson("BuildInType", getJson("Float"));
		break;
	case _Double:
		return getJson("BuildInType", getJson("Double"));
		break;
	case _Void:
		return getJson("BuildInType", getJson("Void"));
		break;
	default:
		break;
	}
}