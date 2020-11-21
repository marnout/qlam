" Vim syntax file
" Language:	Qlam
" Maintainer:	Mourad Arnout
" Last Change:	2014/08/22

" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

syn region 	qlmStyle start="'/" end="/'" fold transparent
syn region 	qlmStyle start="'\*" end="\*'" fold transparent
syn region 	qlmStyle start="'_" end="_'" fold transparent
syn region	qlmCode	 start="`" end="`" fold transparent
syn region  	qlmEndTag start=+</+      end=+>+ fold transparent 
syn region  	qlmTag start=+<[^/]+   end=+>+ fold transparent
"syn region	qlmQuote start="#code" end="\z1" keepend
" Avoid matching "text#text", used in /etc/disktab and /etc/gettytab
syn match 	qlmComment	"^# .*$"
syn match	qlmCmd		"^#[^ ].*$" contains=qlmCmd
syn match	qlmTitle	"^#[1-6] .*$"
"syn keyword	qlmHead contained #meta #style #title
syn keyword	qlmCmd	#inc_html #inc_js #table #code
syn keyword	qlmCmd #menu #substance
"syn keyword	qlmTitle 1 2 3 4 5 6 contained
syn match 	qlmHead		"^#meta .*$"
syn match	qlmHead		"^#style .*$"
syn match	qlmHead		"^#link .*$"
syn match	qlmHead		"^#title .*$"
syn match	qlmHead		"^#js .*$"
syn match 	qlmLink		"\[.\{-}->.\{-}\]"
syn match	qlmItalic	".\{-}"
syn match	qlmBold		".\{-}"
syn match	qlmCode		".\{-}"
syn match	qlmUnderline	".\{-}"
syn region	qlmHtml	start="<" end=">"
syn region	qlmQuote 
  \ start=+[uU]\=\z\(#code\)+ end="^$" 

syn region	qlmJs
  \ start=+[uU]\=\z\(#js\)+ end="^$" 

syn region	qlmSubstance
  \ start=+[uU]\=\z\(#substance\)+ end="^$" 
" Define the default highlighting.
" Only used when an item doesn't have highlighting yet
hi qlmComment	ctermfg=grey
hi qlmHead		ctermfg=grey
hi qlmTitle		ctermfg=white	    ctermbg=black cterm=bold,underline
hi qlmCmd 		ctermfg=lightgrey
hi qlmBold		ctermfg=White	    cterm=bold
hi qlmItalic	ctermfg=white	    cterm=italic
hi qlmLink		ctermfg=white	    cterm=bold
hi qlmUnderline	ctermfg=white	    cterm=underline
hi qlmCode		ctermfg=white	    ctermbg=240
hi qlmJs		ctermfg=lightgrey
hi qlmQuote		ctermfg=grey
hi qlmSubstance	ctermfg=lightCyan
hi qlmHtml	ctermfg=lightgrey

map <F4> :w <bar> :!qlam %< <CR>
map <F5> :!qlam %< <CR>


let b:current_syntax = "qlam"


