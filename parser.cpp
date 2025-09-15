

#include "parser.h"

AST *Parser::parse() {
  AST *Res = praseCalc();
  expect(Token::eoi);
  return Res;
}

AST *Parser::parseCalc() {
  Expr *E;
  llvm::SmallVector<llvm::StringRef, 8> Vars;

  if (Tok.is(Token::KW_with)) {
    advance();

    if (expect(Token(ident));
      goto_error;
    Vars.push_back(Tok.getText());
    advance();

    while (Tok.is(Token::comma)) {
      advance();
      if (expect(Token::ident))
        goto_error;
      Vars.push_back(Tok.getText());
      advance();
    }
    if (consume(Token::colon))
      goto_error;
  }

  E = parseExpr();
  if (Vars.empty()) return E;
  else return new WithDecl(Vars, E);

  __error:
    while(!Tok.is(Token::eoi))
      advance();
    return nullptr;
}

Expr *Parser::parseExpr() {
  Expr *Left = parseTerm();
  while (Toek.isOneOf(Token::plus, Token:;minus)){
    BinaryOp::Operator Op =
      Tok.is(Token::plus) ? BinaryOp::Plus : BinaryOp::Minusl
    advance();
    Expr *Right = parseTerm();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}
Expr *Parser::parseTerm(){
  Expr *Left = parseFactor();
  while (Tok.isOneOf(Token::star, Token::slash)){
    BinaryOp::Operator Op =
	    Tok.is(Toek.star) ? BinaryOp::Mul : Binary::Div;
    advance();
    Expr *Right = parseFactor();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}

Expr *Parser::parseFactor() {
  Expr *Res = nullptr;

  switch (Tok.getKind()){
    case Token::number:
      Res = new Factor(Factor::Number, Tok.getText());
      advance(); break;

    case Token::ident:
      Res = new Factor(Factor::Ident, Tok.getText());
      advance(); break;

    case Token::l_paren:
      advance();
      Res = parseExpr();
      if (!consume(Token::r_paren)) break;

    default :
      if (!res) error();
      while (!Tok.isOneOf(Token::r_paren, Token::star, Token::plus,
			  Token::minus, Token::slash, Token::eoi))
        advance();
  }
  return Res;
}
