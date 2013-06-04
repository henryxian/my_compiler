#! /usr/bin/env python
# -*- coding:utf-8 -*-

#this is a symbol table for lexical analyzer.
#provides constants which represent the numeric
#values of leaf nodes of the parse tree(terminal
#tokens).

import re

ACC          = -2

ENDMARKER    = 0
SEMI         = 1
LESS         = 2
GREATER      = 3
EQUAL        = 4
LESSEQUAL    = 5
NOTEQUAL     = 6
GREATEREQUAL = 7
HASHTAG      = 8
COLON        = 9
LPAR         = 10
RPAR         = 11
PLUS         = 12
MINUS        = 13
MULTIPLY     = 14 
DIVIDE       = 15
LBRACE       = 16
RBRACE       = 17
PERCENT      = 18

KEY_IF    = 19
KEY_ELIF  = 20
KEY_ELSE  = 21
KEY_WHILE = 22
KEY_AND   = 23
KEY_OR    = 24
KEY_IS    = 25
KEY_NOT   = 26
BEGIN     = 27
END       = 28
THEN      = 29

#provides constants which represent the numeric values
#of non-leaf node of the parse tree(non-terminal).

if_stmt         = 258
while_stmt      = 259
and_test        = 260
or_test         = 261
comp_operator   = 262
assignment_stmt = 263

E_1 = 264
E_2 = 265
E_3 = 266
E_4 = 267
E_5 = 268
E_6 = 269


#keywords
keywords = {
			"begin":BEGIN,
			"end":END,
			"if":KEY_IF,
			"elif":KEY_ELIF,
			"else":KEY_ELSE,
			"while":KEY_WHILE,
			"and":KEY_AND,
			"or":KEY_OR,
			"is":KEY_IS,
			"not":KEY_NOT,
			"then":THEN	
			}

#other tokens
other_tokens = {
			"#":HASHTAG,
			";":SEMI,
			">":GREATER,
			"=":EQUAL,
			"<=":LESSEQUAL,
			">=":GREATEREQUAL,
			"=":EQUAL,
			"<>":NOTEQUAL,
			"#":HASHTAG,
			":":COLON,
			"+":PLUS,
			"-":MINUS,
			"*":MULTIPLY,
			"/":DIVIDE,
			"%":PERCENT,
			"{":LBRACE,
			"}":RBRACE,
			"(":LPAR,
			")":RPAR
				}

ACTION = 	{
			"id":[5, -1, -1, -1, 5, -1, 5, 5, -1, -1, -1, -1],
			"+":[-1, 6, E_2, E_4, -1, E_6, -1, -1, 6, E_1, E_3, E_5],
			"*":[-1, -1, 7, E_4, -1, E_6, -1, -1, -1, 7, E_3, E_5],
			"(":[4, -1, -1, -1, 4, -1, 4, 4, -1, -1, -1, -1],
			")":[-1, -1, E_2, E_4, -1, E_6, -1, -1, 11, E_1, E_3, E_5],
			"$":[-1, ACC, E_2, E_4, -1, E_6, -1, -1, -1, E_1, E_3, E_5]
			}

GOTO   =	{
			"E":[1, None, None, None, 8, None, None, None, None, None, None, None],
			"T":[2, None, None, None, 2, None, 9, None, None, None, None, None],
			"F":[3, None, None, None, 3, None, 3, 10, None, None, None, None]
			}

global linecount
#state     = 0
statestack = []
idtable   = []
constants = []
tokens    = []
linecount = 0
other_tokens_set = set(other_tokens.keys())
special_set = set(["<", ">", "<>", ">=", "<="])
others = other_tokens_set - special_set

if __name__ == '__main__':
	with open("source.xian", "r") as xian:
		readlines = xian.readlines()
		#print readlines
		count = 0
		for line in readlines:
			linecount = len(readlines)
			line.rstrip()
			line = line.split()
			count += 1
			for word in line:
				tok = ""
				while word:
					if word[0] == "<":
						word = word[1:]
						if len(word):
							if word[0] == "=":
								word = word[1:]
								tokens.append(("<=", LESSEQUAL))
							elif word[0] == ">":
								word = word[1:]
								tokens.append(("<>", NOTEQUAL))
							else:
								tokens.append(("<", LESS))
						else:
							tokens.append(("<", LESS))
						continue
					if word[0] == ">":
						word = word[1:]
						if len(word):
							if word[0] == "=":
								word = word[1:]
								tokens.append((">=", GREATEREQUAL))
							else:
								tokens.append(">", GREATER)
						else:
							tokens.append((">", GREATER))
						continue
					if word[0] in others:
						tokens.append((word[0], other_tokens[word[0]]))
						word = word[1:]
						continue
					if word[0].isalpha():
						tok = tok + word[0]
						word = word[1:]
						while word:
							if word[0].isalnum():
								tok = tok + word[0]
								word = word[1:]
							else:
								break
						if tok in keywords:
							tokens.append((tok, keywords[tok]))
						else:
							if tok not in idtable:
								idtable.append(tok)
							tokens.append(("id", tok))
							tok = ''
						continue
					if word[0].isdigit():
						tok = tok + word[0]
						word = word[1:]
						while word:
							if word[0].isdigit():
								tok = tok + word[0]
								word = word[1:]
						constants.append(("const", tok))
						tokens.append(("const", tok))
						tok = ''
						continue
					else:
					 	print "Error in line", count
					 	break
	# print "idtable:"
	# print idtable
	# print "constants:"
	# print constants
	# print "tokens:",tokens
	# print "Total lines of the source file:", linecount
	tokens.append(("$", "$"))
	tokens.reverse()
	# print tokens
	x, y = tokens.pop()
	print x, y
	statestack.append(0)
	while True:
		if not len(statestack):
			statestack.append(0)
		top = statestack[-1]
		if ACTION[x][top] < 30 and ACTION[x][top] != -1 and ACTION[x][top] != ACC:
			statestack.append(ACTION[x][top])
			if  len(tokens):
				x, y = tokens.pop()
				print x, y
		elif ACTION[x][top] <= E_6 and ACTION[x][top] >= E_1:
			state = ACTION[x][top]
			if state == E_1:
				for i in range(3):
					statestack.pop()
				top2 = statestack[-1]
				statestack.append(GOTO["E"][top2])
				print "E->E+T"
			elif state == E_2:
				statestack.pop()
				top2 = statestack[-1]
				statestack.append(GOTO["E"][top2])
				print "E->T"
			elif state == E_3:
				for i in range(3):
					statestack.pop()
				top2 = statestack[-1]
				statestack.append(GOTO["T"][top2])
				print "T->T*F"
			elif state == E_4:
				statestack.pop()
				top2 = statestack[-1]
				statestack.append(GOTO["T"][top2])
				print "T->F"
			elif state == E_5:
				for i in range(3):
					statestack.pop()
				top2 = statestack[-1]
				statestack.append(GOTO["F"][top2])
				print "F->(E)"
			elif state == E_6:
				statestack.pop()
				top2 = statestack[-1]
				statestack.append(GOTO["F"][top2])
				print "F->id"
		elif ACTION[x][top] == ACC:
				print "Accept!"
				break
		else:
			print "Error!"
			break
				



	




						


			




