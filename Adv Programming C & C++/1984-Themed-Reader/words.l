%option noinput
%option nounput
%option noyywrap

ANY 			[A-Za-z_0-9~`!@#$%^&*()_+-={}|\\\[\]:\"\'<>,./?;]
LETTER          [A-Za-z_]
DIGIT           [0-9]
WHITESPACE      [ \t\r\n]
PUNCTUATION     [~`!@#$%^&*()_+-={}|\\\[\]:;\"\'<>,./?]

%%

{ANY}+{WHITESPACE}                   {return 0;}
{LETTER}+"'"({LETTER}|{LETTER}{LETTER}) {return 0;}
{LETTER}+{DIGIT}+                       {return 0;}
{PUNCTUATION}+                          {}
{WHITESPACE}+                           {}
<<EOF>>                                 { return -1; }
.                                       {}

%%