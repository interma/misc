" An example for a vimrc file.
"
" Maintainer:	Bram Moolenaar <Bram@vim.org>
" Last change:	2002 Sep 19
"
" To use it, copy it to
"     for Unix and OS/2:  ~/.vimrc
"	      for Amiga:  s:.vimrc
"  for MS-DOS and Win32:  $VIM\_vimrc
"	    for OpenVMS:  sys$login:.vimrc

" filetype plugin on
filetype plugin on

" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

" Use Vim settings, rather then Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

if has("vms")
  set nobackup		" do not keep a backup file, use versions instead
else
  set backup		" keep a backup file
endif
set history=50		" keep 50 lines of command line history
set ruler		" show the cursor position all the time
set showcmd		" display incomplete commands
set incsearch		" do incremental searching

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq

" This is an alternative that also works in block mode, but the deleted
" text is lost and it only works for putting the current register.
"vnoremap p "_dp

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " Put these in an autocmd group, so that we can delete them easily.
  augroup vimrcEx
  au!

  " For all text files set 'textwidth' to 78 characters.
  autocmd FileType text setlocal textwidth=78

  " When editing a file, always jump to the last known cursor position.
  " Don't do it when the position is invalid or when inside an event handler
  " (happens when dropping a file on gvim).
  autocmd BufReadPost *
    \ if line("'\"") > 0 && line("'\"") <= line("$") |
    \   exe "normal g`\"" |
    \ endif

  augroup END

else

  set autoindent		" always set autoindenting on

endif " has("autocmd")


" my setting
filetype plugin on
syntax on
au BufNewFile,BufRead *.t2t set ft=txt2tags
au FileType python source ~/.vim/plugin/python.vim 
"autocmd FileType python setlocal et sta sw=4 sts=4
au Filetype html,xml,xsl,rhtml source ~/.vim/scripts/closetag.vim

let g:winManagerWindowLayout = "TagList" 
"let g:winManagerWindowLayout='FileExplorer|TagList'
nmap wm :WMToggle<CR>

let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplTabWrap = 1


set laststatus=2
function! CurDir()
  let curdir = substitute(getcwd(), '/Users/amir/', "~/", "g")
  return curdir
endfunction
set statusline=\ %F%m%r%h\ %w\ \ CWD:\ %r%{CurDir()}%h\ \ \ Line:\ %l/%L:%c

set nofen
set fdl=0


set fencs=ucs-bom,utf-8,gb18030,gbk,gb2312,cp936

set guifont=Bitstream\ Vera\ Sans\ Mono\ 10
set nocp
colorscheme advantage
"colorscheme desert

"set sw=4
"set ts=4
""set expandtab
set ts=4 sw=4 sts=4 tw=100 
"set ts=4 sw=4 sts=4 

set noet
set vb t_vb=
set dir=~/temp
set backupdir=~/temp

set tags=./tags,/usr/include/tags,tags;

set cscopequickfix=s-,c-,d-,i-,t-,e-
cs add cscope.out 
set completeopt=longest,menu


set whichwrap=b,s,<,>,[,]
set foldmethod=syntax
" set ai
set nu
set noshowmatch
set lbr
set cino=:0g0t0(sus

" my key map
nmap <F2> :w<CR>
imap <F2> <ESC><F2><CR>
" nmap <C-S> :w<CR>
" imap <C-S> <ESC><C-S><CR>
nmap <F3> :nohlsearch<CR>
nmap <F4> <C-W>w
nmap ww <C-W>w
imap <F4> <ESC><F4>
nmap <F5> :q<CR>
imap <F5> <ESC><F5>
nmap <F6> :make<CR>
imap <F6> <ESC><F6>
imap jj <ESC>
nmap ww :w<CR>
imap ww <ESC>:w<CR>
"nmap <C-J> :tabn<CR>
"imap <C-J> <ESC><C-J><CR>
"nmap <C-K> :tabp<CR>
"imap <C-K> <ESC><C-K><CR>
"imap <C-J> <ESC><C-J><CR>
"nmap <C-K> :tabp<CR>
"imap <C-K> <ESC><C-K><CR>
nmap <C-j> :bn<cr> 
nmap bn :bn<cr> 
nmap <C-k> :bp<cr>
nmap bp :bp<cr> 

" cscope key
nmap <C-_>s :cs find s <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>g :cs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>c :cs find c <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>t :cs find t <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>e :cs find e <C-R>=expand("<cword>")<CR><CR>
nmap <C-_>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
nmap <C-_>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
nmap <C-_>d :cs find d <C-R>=expand("<cword>")<CR><CR>
" A
nnoremap <silent> <F12> :A<CR>

map <F10> :set paste<CR>
map <F11> :set nopaste<CR>
imap <F10> <C-O>:set paste<CR>
imap <F11> <nop>
set pastetoggle=<F11>

map <F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>

" alias
iabbrev uns; using namespace std;

" c-support setting
let g:C_AuthorName      =   'interma'     
"let g:C_AuthorRef       =   'IM'                         
let g:C_Email           =   'interma@outlook.com'           
" doxygenToolkit setting
let g:doxygenToolkit_authorName="InterMa"
let g:doxygenToolkit_briefTag_funcName="yes"
" supertab
let g:SuperTabRetainCompletionType=0
" omni
let OmniCpp_DefaultNamespaces = ["std"]
let OmniCpp_NamespaceSearch = 1
let OmniCpp_MayCompleteDot = 1
let OmniCpp_MayCompleteArrow = 1
let OmniCpp_MayCompleteScope = 1
let OmniCpp_ShowScopeInAbbr = 1
let OmniCpp_ShowPrototypeInAbbr = 1

set autowrite
