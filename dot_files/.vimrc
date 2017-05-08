"--------------------------------------------------
" Character encoding
set termencoding=utf-8        
set encoding=utf-8 fileencodings=ucs-bom,utf-8,cp936,gbk,big5,latin1


"--------------------------------------------------
" Base settings
set guifont=Fixedsys\ 10 
set nocompatible          " We're running Vim, not Vi!
syntax on                 " syntax highlingt
set autoindent            " automatical code indent
set cindent
set smartindent
set backspace=2           " backspace del all
set history=50            " history of commands
set hlsearch              " highlignt search fraze
filetype plugin on        " file type based syntax highliht
filetype indent on
filetype on
set ts=4                  " tab stop
set sw=4                  " shift width (with autoindent)
set ai!
set textwidth=2048        " text witdth
set nu                    " show numbers
set lcs=tab:>.,eol:$      " show non printed chars
set whichwrap+=<,>,[,]    " where wrap long lines
set gdefault              " /g default on s///
set showcmd               " show typed commands
"set wildmenu              " show menu (bash-like) on tab
set wildignore=*.o,*~     " ignor on wildmenu
set incsearch             " show what is typed on /
"set viminfo='1000,f1,:1000,/1000  " big viminfo :)
set history=500
set lazyredraw
set wmh=0
set t_Co=256              " color numbers
set wrap                " do not wrap lines
" 代码折叠
" set foldmethod=marker
" set foldmethod=syntax

"--------------------------------------------------
" Change encoding
""map _u :set encoding=utf-8<cr>:set fileencodings^=utf-8<cr>:set termencoding=utf-8<cr>
""map _i :set encoding=iso-8859-2<cr>:set fileencodings^=iso-8859-2<cr>:set termencoding=iso-8859-2<cr>

"--------------------------------------------------
" Non indend paste: <F11><S-Ins><F11>
set pastetoggle=<F11>

"--------------------------------------------------
" Open in last edit place
au BufReadPost * if line("'\"") > 0 && line("'\"") <= line("$") | exe "normal g'\"" | endif

"--------------------------------------------------
" Syntax coloring (~/.vim/colors/)
" available colors: `ls /usr/share/vim/vim??/colors`
" colorscheme summerfruit256 
colorscheme herald 

"---------------------------------------------------
" Tab size
:set tabstop=4        " Force tabs to be displayed/expanded to 2 spaces (instead of default 8).
:set softtabstop=4    " Make Vim treat <Tab> key as 2 spaces, but respect hard Tabs.
                      " I don't think this one will do what you want.
:set expandtab        " Turn Tab keypresses into spaces.  Sounds like this is happening to you.
                      " You can still insert real Tabs as [Ctrl]-V [Tab].
:set shiftwidth=4     " When auto-indenting, indent by this much.
                      " (Use spaces/tabs per "expandtab".)
:retab                " Change all the existing tab characters to match the current tab settings

"状态行颜色
hi statusline term=underline ctermbg=darkgray ctermfg=gray
"在状态行上显示光标所在位置的行号和列号 
set ruler 
set rulerformat=%20(%2*%<%f%=\ %m%r\ %3l\ %c\ %p%%%) 
set laststatus=2             " 开启状态栏信息
set cmdheight=1              " 命令行的高度，默认为1，这里设为2
set statusline=%F%m%r%h%w\ \[POS=%l,%v][%p%%]\%{strftime(\"%Y/%m/%d\ %H:%M\")} 

"多标签浏览设置标签颜色
highlight TabLine term=underline ctermbg=darkgray ctermfg=gray
highlight TabLineSel term=bold cterm=bold ctermbg=darkgreen ctermfg=white
"高亮光标所在行
set cursorline
"hi CursorLine cterm=NONE ctermbg=darkgray 
"hi CursorColumn cterm=NONE ctermbg=darkred ctermfg=white

""""""""""""""""""""""""""""""PLUGIN""""""""""""""""""""""""""""

let g:winManagerWindowLayout = "TagList" 
"let g:winManagerWindowLayout='FileExplorer|TagList'
nmap wm :WMToggle<CR>
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplTabWrap = 1

set noet
set vb t_vb=
set dir=~/temp
set backupdir=~/temp

set tags=./tags,/usr/include/tags,tags;

"set cscopequickfix=s-,c-,d-,i-,t-,e-
"cs add cscope.out 


set completeopt=longest,menu
set whichwrap=b,s,<,>,[,]
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
nmap <F4> :make<CR>
"nmap <F4> <C-W>w
"nmap ww <C-W>w
"imap <F4> <ESC><F4>
map <F5> :set paste<CR>
map <F6> :set nopaste<CR>
set pastetoggle=<F5>
imap aa <ESC>
nmap aw :w<CR>
 "imap jw <ESC>:w<CR>
nmap <C-j> :bn<cr> 
nmap bn :bn<cr> 
nmap <C-k> :bp<cr>
nmap bp :bp<cr> 

" move in insert quickly

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
execute pathogen#infect()
call pathogen#helptags()


" Specify a directory for plugins (for Neovim: ~/.local/share/nvim/plugged)
call plug#begin('~/.vim/plugged')

" Make sure you use single quotes

" Multiple Plug commands can be written in a single line using | separators
Plug 'SirVer/ultisnips'
Plug 'honza/vim-snippets'

" On-demand loading
Plug 'scrooloose/nerdtree', { 'on':  'NERDTreeToggle' }

" Plugin outside ~/.vim/plugged with post-update hook
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }

" :FixWhitespace
Plug 'bronson/vim-trailing-whitespace'

Plug 'hynek/vim-python-pep8-indent'
Plug 'ervandew/supertab'

" Initialize plugin system
call plug#end()

