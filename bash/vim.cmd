
Normal mode:
f{character}  find the character in this line, ; will find next,  , will find previous.
t{char}
ga    print the ascii value of the character.
<C -v>  into visual colum block mode
gv	reselect previous highlight block.
.    repeat previous modification.
@:    repeat previous previous Ex cmd.
:h digraph-table  show the digraph table.
:h ex-cmd-index	  show all ex cmd list.
:{range} normal {cmd}:  execute normal cmd at target lines.
    :{range} normal A;    all ; at the end of lines.
    :{range} normal i//   add // at the begih of lines.
q:    into Ex cmd window.
gj/gk    move curse based the display line.
e	move to the end of word.
ge      back move to the end of word.
W/B/E/gE    use WORD as unit
m{a-zA-Z}    mark current cursor position
`{a-zA-Z}    move to target mark
%	move the another part of closures.
<C -o>/<C -i>  go back / forward, not cross tables.
g;	move to previous modified position
gi	return last insert position
{"register name}x/s/y/d/c/p {motion}	"0 is only for copy, "" is nameless register. 
		"+ is system clipboard, "* is clipboard of highlight.
q{register}	record macro. q exit.
{number}@{register}	replay macro number times.
0	move to begin of line.
<C -l>		redraw screen.
zz	move screen to center the cursor.
~	switch char case.



Visual mode:
o    switch highlight block activated directions.


All mode:
<C -[>  equal to Esc

Insert mode:
<C -v>{code}  insert character according its character code.
<C -v>u{code} insert character according its unicode.
<C -k>{char1}{char2}  insert character according its digraph.
<C -r>{register}    insert register's content.

Ex mode:
<C -d>   can list all candidate cmd.
<C -r><C -w>  insert the word under cursor.
<C -f>  switch into Ex cmd window.
!{bash_cmd}  execute bash cmd
shell    into shell.
read !{bash_cmd}  execute bash cmd and insert its output below cursor.
[range]write !{bash_cmd}  execute bash cmd and use the range lines as its input.
[range]!{filter}    use outside filter to filt target lines, such as 'sort'
tabedit {filename}  open a new tabel. gt/gT/{N}gt to switch tables.
changes		show the change list which include all modifies we have done.
g/pattern/{action} execute action on matched lines.
	g/pattern/d delete the matched line.
	pattern: \V{pattern} char has its original mean. \v{pattern} like python.
	g/^\s*$/d: delete blank lines.
v/pattern/{action}  execute action non-matched lines.

set spell	spell checker.[s/]s goto next/pre spell error. z= provide suggest.
		zg  add into dictionary, zw remove from dictionary, zug  undo zg/zw.

ls	list all buffers
bnext/bprevious/bfirst/blast	switch buffers
buffer N	switch to target buffer
bdelete N1 N2	deletet target buffers



config:
~/.vim/vimrc: set "syntax enable\n syntax on", syntax file should put into ~/.vim/syntax/ dir

Set colorscheme
colorscheme <ron, zellner, pablo>
