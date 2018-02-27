\ProvidesClass{temp}[2016/02/01 CV]
\NeedsTeXFormat{LaTeX2e}
\DeclareOption{print}{\def\@cv@print{}}
\DeclareOption*{%
  \PassOptionsToClass{\CurrentOption}{scrartcl}
}
\ProcessOptions\relax
\LoadClass{scrartcl}

% \usepackage[english]{babel}
\usepackage[UTF8]{ctex}
\usepackage[utf8x]{inputenc}
\usepackage[default,scale=0.95]{opensans}
\usepackage[T1]{fontenc}

%\usepackage[protrusion=true,expansion=true]{microtype}
\usepackage{amsmath,amsfonts,amsthm}    
\usepackage{graphicx}
\usepackage{wrapfig}
\usepackage[hmargin=1.25cm, vmargin=0.7cm]{geometry}
\usepackage[usenames,dvipsnames]{xcolor}

\usepackage{sidecap}
\usepackage{geometry}
\usepackage{url}
\usepackage{float}
\usepackage{enumitem}
\usepackage{fontawesome}
\usepackage{tikz}
\usetikzlibrary{math,positioning,arrows,calc,shapes}
%% 定义文本节点
\tikzset{%
	textNodeStyle/.style={align=left,
			inner sep=0pt, minimum size=2em,
			outer sep=1pt, #1},
	textNodeStyle/.default={},
	boxStyle/.style={line width=1pt,%
			rounded corners=3pt,
		}% 还可以加颜色
}

\usepackage[xetex, colorlinks=true, urlcolor=DarkSlateBlue,
citecolor=DarkSlateBlue, filecolor=DarkSlateBlue, plainpages=false,
pdfpagelabels, bookmarksnumbered]{hyperref}

\graphicspath{{IMG/}}
%% 文本节点与图像节点
\newcommand{\tNode}[3]{\node[textNodeStyle] (#1) at #2 {#3}}
\newcommand{\tNodePos}[5]{\node[textNodeStyle,#2=of #3, #2=0pt, shift={#4}] (#1) {#5}}
\newcommand{\figNode}[4]{\node(#1) at #4 			
			{\includegraphics[width=#3,keepaspectratio]{#2}}}
\newcommand{\figNodePos}[6]{\node[#4=0pt,#4=of #5,shift={#6}](#1)
			{\includegraphics[width=#3,keepaspectratio]{#2}}}
			
\newcommand{\mtNode}[7]{
	\begin{tikzpicture}
		\node[textNodeStyle={text width=#2}]
			(#1) at #3 {#4};
		\figNodePos{f1}{#5}{#6}{right}{#1}{#7};
	\end{tikzpicture}
}
\newcommand{\mtNodePos}[6]{\tikz\node[textNodeStyle={text width=#2},
	#3=of #4, #3=0pt, shift={#5}] (#1) {#6};}	

%\definecolor{LinkColor}{HTML}{554CF3}
%\usepackage[hidelinks]{hyperref}
%\AtBeginDocument{%
%	\hypersetup{%
%		colorlinks=true,
%		bookmarksnumbered=ture,
%		filecolor=LinkColor%	
%	}%
%}


%\frenchspacing              % Better looking spacings after periods
\pagestyle{empty}           % No pagenumbers/headers/footers

% Color definitions
\definecolor{date}{HTML}{666666} 
\definecolor{primary}{HTML}{191919} 
\definecolor{headings}{HTML}{025178}
\definecolor{subheadings}{HTML}{002648}


%%% Custom sectioning (sectsty package)
%%% ------------------------------------------------------------
\usepackage{sectsty}

%\sectionfont{\usefont{OT1}{phv}{b}{n}\sectionrule{0pt}{0pt}{-5pt}{3pt}}
\sectionfont{\usefont{OT1}{phv}{b}{n}}
\subsectionfont{\color{headings}\usefont{OT1}{phv}{b}{n}}
\subsubsectionfont{\sectionrule{0pt}{0pt}{-5pt}{3pt}}


%%%
\setlist{%
  topsep=0.3em, 
  partopsep=0pt, 
  itemsep=0ex plus 0.1ex, 
  parsep=0pt, 
%  leftmargin=1.5em,
  leftmargin=2em, 
  rightmargin=0em, 
  labelsep=0.5em, 
  labelwidth=2em 
}


%%% Macros
%%% ------------------------------------------------------------
\newlength{\spacebox}
\settowidth{\spacebox}{8888888888}			% Box to align text
\newcommand{\sepspace}{\vspace*{1em}}		% Vertical space macro

\newcommand{\MyName}[1]{ % Name 
		 \zihao{1} \usefont{OT1}{phv}{b}{n} \hfill \textcolor{headings}{#1}
		\par \normalsize \normalfont}

%\newcommand{\NewPart}[1]{\section*{ \color{headings} \uppercase{#1}}}
\newcommand{\NewPart}[1]{\vspace*{-.1cm}\section*{ \color{headings} \uppercase{#1}}\vspace*{-.7cm}\tikz\shade[left color=headings,right color=white] (0, 0) rectangle (\linewidth, .08);\vspace*{.2cm}}

\newcommand{\PersonalEntry}[2]{
		\noindent\hangindent=2em\hangafter=0 % Indentation
		\parbox{\spacebox}{        % Box to align text
		\textit{#1}}		       % Entry name (birth, address, etc.)
		\hspace{1.5em} #2 \par}    % Entry value

\newcommand{\SkillsEntry}[2]{      % Same as \PersonalEntry
		\noindent\hangindent=2em\hangafter=0 % Indentation
		\parbox{\spacebox}{        % Box to align text
		\textit{#1}}			   % Entry name (birth, address, etc.)
		\hspace{1.5em} #2 \par}    % Entry value	
        
\newcommand{\keyword}{\textcolor{subheadings}{关键词：}}
   
\newcommand{\projectEntry}[8]{%
		\noindent%
		\hspace*{1em}%	                    
        \begin{minipage}{0.96\linewidth}  
    	\noindent {\color{subheadings}\bfseries {#1}}， {\color{subheadings}\textit{#3}}  
        \hfill  {\color{headings}\fontsize{10pt}{12pt}#2}   
        \par\vspace*{.5em} 	
        \noindent\hangindent=2em\hangafter=0 \normalsize
        \hspace*{-1.2em}
        \mtNode{t1}{.8\textwidth}{(0, 0)}{%
         #4 % Description
         \par
         \small\keyword #5
         }{#6}{#7}{#8}%
		 \normalsize\par\vspace*{-.4em}         
        \end{minipage}         
}
        
\newcommand{\EducationEntry}[3]{%
		\noindent%
		\hspace*{1em}%                   
        \begin{minipage}{0.96\linewidth}  
    	\noindent {\color{subheadings}\bfseries\uppercase {#1}} 
        \hfill {\color{headings}\fontsize{10pt}{12pt}#2}
		\par   	
%		\noindent {\color{subheadings}\textit{#3}} \par 
%        \noindent\hangindent=0em\hangafter=0 
        \normalsize #3 % Description
		\normalsize \par         
        \end{minipage}        
 }

\newcommand{\flag}[1]{
	\includegraphics[height=9pt, width= 15pt]{#1}
}     

\newcommand{\software}[1]{
	\includegraphics[height=9pt]{#1}
}  

\newcommand{\softwareb}[1]{
	\raisebox{-.6\height}{\includegraphics[height=18pt]{#1}}
}