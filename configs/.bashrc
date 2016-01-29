HISTCONTROL=ignoredups:ignorespace

# append to the history file, don't overwrite.
shopt -s histappend

HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command.
shopt -s checkwinsize

alias grep='grep --color=auto'
alias fgrep='fgrep --color=auto'
alias egrep='egrep --color=auto'
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

export GOPATH=$HOME/workspace
export PATH=$PATH:/usr/local/go/bin