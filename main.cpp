#include "common.h"
#include "lex.yy.cpp"

Qtree::Qtree( int t){
	type = t;
	left = NULL;right = NULL;
}
Qexpression::Qexpression( int t, string s){
	type = t; 
	str = string(s) ;
	left = right = NULL;
}
Qexpression::Qexpression( int t, int n){
	type = t;
	number = n ;
	left = right = NULL;
}
Qexpression::Qexpression( int t, int p, string s){
	type = t;
	number = p;
	str = string(s);
	left = right = NULL;
}
bool Qexpression::judge(Tuple *t ) {
	Qexpression *r = this->right ;
	Qexpression *l = this->left ;
	int rt = r->type ;
	int lt = l->type ;
	string s ; 
	switch(type){
	case COMPARE:{
		if( rt == COLUMN && lt == COLUMN) {
		}else if (rt == COLUMN ){
			int found = r->str.find('.');
			union Field fld ;
			if(found == std::string::npos){
				string s(r->str) ;
			}else{
				string s( r->str.substr(found + 1) ) ;
			}
			fld = t->getField(s) ;//Same name TODO

			if(lt == LITERAL){
				if (this->str[0] == '='){
					return (l->str.compare(*(fld.str) ) == 0) ;
				}
			}else if(lt == INTEGER) {
				if (this->str[0] == '='){
					return fld.integer == l->number ;
				}else if(this->str[0] == '>'){
					return fld.integer > l->number ;
				}else if(this->str[0] == '<'){
					return fld.integer < l->number ;
				}
			}else {
				perror("Type error: should be INTEGER or LITERAL") ;
				exit(EXIT_FAILURE);
			}
		}else if (lt == COLUMN) {
			
		}else{
		}	
	}
	break;
	}
}
void Qexpression::free(){
	if(left != NULL){left->free();}
	if(right!=NULL){right->free();}
	delete this;
}
void Qexpression::print(int level ){
	int i ;
	for ( i = level ; i > 0 ; i --){ cout<< "\t" ;}
	if (type == OPERATER){
		cout << str << endl ;
		if(str[0] == 'N'){
			this->left->print(level + 1); 
		}else{
			this->left->print(level + 1);
			this->right->print(level + 1) ;
		}
	}else if (type == LITERAL || type == COLUMN){
		cout << str << endl;
	}else if ( type == INTEGER ){
		cout << number << endl;
	}
}
void Qtree::print(int level ){
	int i ;

	for ( i = level ; i > 0 ; i --){ cout<< "\t" ;}
	switch(type) {
		case PI: cout << "π " << "\t["; break;
		case SIGMA: cout << "σ " << "\t[";break;
		case DELTA: cout << "δ " << "\t[";break;
		case JOIN: cout << "X " << "\t["; break;
		case TAU: cout << "τ" << "\t["; break; 
	}
	for(i = 0; i < info.size(); i++){
		cout << info[i] << " " ;
	} cout << "]" << endl ;
	if (left != NULL){left->print(level + 1) ;}
	if(right != NULL){ right->print(level + 1) ; }
}

void Qtree::free(){
	info.clear();
	if(left != NULL){left->free();}
	if(right!=NULL){right->free();}
	delete this;
}
void Qtree::exec(){
	print(0);
}

void err_out_START(const char str[]) {
	if(YY_START == D_S_EXPECT_WHERE) {
		scan_over(DELETE_STATEMENT) ;
	}
	#ifndef  NO_ERROR_OUTPUT
	cerr << GREEN_TEXT << HLINE << endl << "Scan start:\t" <<
	str << NORMAL_TEXT << endl;
	#endif
}
void scan_over(int statement){
	char buf[64];
	switch(statement){
	case CREATE_TABLE_STATEMENT:
	strcpy(buf, "CREATE TABLE") ;
	break;

	case INSERT_STATEMENT:
	strcpy(buf, "INSERT INTO") ;
	break;

	case DELETE_STATEMENT:
	strcpy(buf, "DELETE FROM") ;
	break;

	case DROP_TABLE_STATEMENT:
	strcpy(buf, "DROP TABLE") ;
	break;

	case SELECT_STATEMENT:
	strcpy(buf, "SELECT [DISTINCT]") ;
	break;

	default:
	perror("unknow statement over");
	exit(EXIT_FAILURE);
	break;
	}
	
	#ifndef  NO_ERROR_OUTPUT
	cerr << RED_TEXT << "Scan over:\t" <<
	buf <<  endl << 
	HLINE << NORMAL_TEXT <<  endl  ;
	#endif
}
int noperands(string s){
	switch(s[0]){
	case '*':
	case '/':
	case '+': 
	case '-':
	case '=':
	case '<':
	case '>':
	case 'A':
	case 'O':
	return 2;
	break;

	case 'N':
	return 1;
	break; 
	}
}
int precedence(string s){
	switch(s[0]){
	case '*':
	case '/':
	return TIMES_DIVIDES;
	break;
	
	case '+': 
	case '-':
	return PLUS_MINUS;
	break;

	case '=':
	case '<':
	case '>':
	return COMPARE;
	break;
	
	case 'N':
	return NOT_PCD;
	break; 

	case 'A':
	return AND_PCD;
	break;

	case 'O':
	return OR_PCD ;
	break;
	}
}
int main( int argc, char **argv ){
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 ){
		yyin = fopen( argv[0], "r" );
	}else{
		yyin = stdin;
	}
	yylex();
}
