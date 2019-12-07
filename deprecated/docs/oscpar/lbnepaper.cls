% -*- latex -*-
% Document class for LBNE papers.
% 
% Many features are supplied in separate .tex files under lbnepaper/
%
% Use it like:
%
%  \documentclass{lbne}
%  \pdfoutput=1            % must be in first 5 lines so arXiv finds it
%  % include certain features
%  \include{lbnepaper/preamble-feature.tex}
%  ...
%  \begin{document}
%  ...
%  \end{document}
%
% Built-in features:
%  - extend the "report" class
%  - up to sub*4 subsections
%  - abstract and bibliography tweaks
%  - various common usepackages
%
% Adapted from LBNE CDR and before that, Daya Bay CDR.
% v2: bv@bnl.gov

\typeout{++++++++++++++++}
\typeout{LBNE Paper Class}
\typeout{++++++++++++++++}
\typeout{}

\def\fileversion{2.0}
\def\filedate{2013/10/14}
\def\docdate {2013/10/14}

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{lbnepaper}[\filedate\space Version \fileversion]
\DeclareOption{letterpaper}
   { \AtEndOfClass{\setlength{\topmargin}{-1.mm}}
    \typeout{>> US letter size paper option}}
\DeclareOption{a4paper}
   { \AtEndOfClass{\setlength{\topmargin}{15.mm}}
    \typeout{>> European A4 paper size option}}

\DeclareOption*
{\typeout{>> Passing option unknown to lbnepaper.cls to Report Class}}

\ProcessOptions

\PassOptionsToClass{\CurrentOption}{report}
%
% make a class file by customization of a given general class
\LoadClass[12pt,letterpaper,twoside]{report}
% Note, the report class does not set \topmargin
% but the size10.cls option will set \topmargin.
% BUT the \AtEndOfClass{..} will then over-ride \topmargin.
\RequirePackage{times}

\newlength{\titleblockheight}        % so user can change it if need be
\setlength{\titleblockheight}{3.0cm}

            %%%%%%%%%%%%%%%%%%%
            % Set the margins %
            %%%%%%%%%%%%%%%%%%%

\setlength{\oddsidemargin}{0.25in}
\setlength{\evensidemargin}{-0.25in}
\setlength{\topmargin}{-2.0cm} 
\setlength{\marginparwidth}{0in}
\setlength{\marginparsep}{0in}

% change where the footer lands.  In general, textheight needs co-adjustment.
%\setlength{\footskip}{0.5in}
\setlength{\textheight}{8.8in} 
\setlength{\textwidth}{6.5in}
\setlength{\headheight}{0.5in}
\setlength{\headsep}{0.5in}

%\setlength{\parindent}{1.0cm}  changed by Anne 9/8/11
\setlength{\parindent}{0cm}
%\setlength{\baselineskip}{2.6ex}
\addtolength{\parskip}{0.5\baselineskip}

\frenchspacing   %suppresses the space at periods that are not full stops.

            %%%%%%%%%%%%%%%%%%%%%%%%%
            % MAKETITLE LINE:       %
            % Define the title page %  
            %%%%%%%%%%%%%%%%%%%%%%%%%

\def\maketitle{\par
 \begingroup
 \def\thefootnote{\fnsymbol{footnote}}
 \def\@makefnmark{\mbox{$^\@thefnmark$}}
 \@maketitle 
 \@thanks
 \endgroup
 \setcounter{footnote}{0}
 \let\maketitle\relax
 \let\@maketitle\relax
 \gdef\@thanks{}\gdef\@author{}\gdef\@title{}\let\thanks\relax}
\def\@maketitle{
{\hsize\textwidth             
 \linewidth\hsize \centering
 {\normalsize \bf \@title \par} \vskip 0.3cm  {\normalsize  \@author \par}}}
\def\copyrightspace{\footnotetext[0]{\mbox{}\vrule height 97pt width 0pt}}

\def\thefootnote{\mbox{\noindent$\fnsymbol{footnote}$}}
    \long\def\@makefntext#1{\noindent$^{\@thefnmark}$#1}
            

 \def\section{\@startsection{section}{1}{\z@}
 {-2.6ex plus -.8ex minus -.17ex}{1.2ex plus .17ex} %{\parindent}
 {\bf\Large\sffamily}}

 \def\subsection{\@startsection{subsection}{2}{\z@}
 {-2.6ex plus -.8ex minus -.17ex}{1.2ex plus .17ex}
 {\bf\normalsize\sffamily}}

 \def\subsubsection{\@startsection{subsubsection}{3}{\z@}
 {-2.6ex plus -.8ex minus -.17ex}{1.2ex plus .17ex} 
 {\bf\normalsize\sffamily}}

 % \def\subsubsubsection{\@startsection{subsubsubsection}{4}{\z@}
 % {-2.6ex plus -.8ex minus -.17ex}{1.2ex plus .17ex} 
 % {\normalsize\bf}}

 % \def\subsubsubsubsection{\@startsection{subsubsubsubsection}{5}{\z@}
 % {-2.6ex plus -.8ex minus -.17ex}{1.2ex plus .17ex} 
 % {\normalsize\bf}}

\newcounter{subsubsubsection}[subsubsection]
\def\subsubsubsectionmark#1{}
\def\thesubsubsubsection {\thesubsubsection
     .\arabic{subsubsubsection}}
\def\subsubsubsection{\@startsection
     {subsubsubsection}{4}{\z@} {-3.25ex plus -1
     ex minus -.2ex}{1.5ex plus .2ex}{\bf\normalsize\sffamily}}
\def\l@subsubsubsection{\@dottedtocline{4}{4.8em}
     {4.2em}}

\newcounter{subsubsubsubsection}[subsubsubsection]
\def\subsubsubsubsectionmark#1{}
\def\thesubsubsubsubsection {\thesubsubsubsection
     .\arabic{subsubsubsubsection}}
\def\subsubsubsubsection{\@startsection
     {subsubsubsubsection}{5} {\z@} {-3.25ex plus -1
     ex minus -.2ex}{1.5ex plus .2ex}{\bf\normalsize\sffamily}}
\def\l@subsubsubsubsection{\@dottedtocline{5}
     {5.8em}{5.2em}}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

            %%%%%%%%%%%%%%%%
            % Bibliography %
            %%%%%%%%%%%%%%%%
 %Centred Title. Parenthesis around the citations (1).

\def\thebibliography#1{
\section*{{\hfill {\normalsize \bf REFERENCES }\hfill
\rule{0pt}{0pt}}\@mkboth
 {REFERENCES}{REFERENCES}}
\list
 {{\arabic{enumi}.}}{\settowidth\labelwidth{{[#1]}}%
 \leftmargin\labelwidth  \frenchspacing
 \advance\leftmargin\labelsep
 \itemsep=-0.2cm
 \usecounter{enumi}}
 \def\newblock{\hskip .11em plus .33em minus -.07em}
 \sloppy
 \sfcode`\.=1000\relax}
%\def\@cite#1#2{\unskip\nobreak\relax{[#1]}}
\let\endthebibliography=\endlist
\def\notbj#1{\mathpalette\c@ncel#1}
\def\diracs#1{\mathpalette\c@ncel#1}
\def\@startsection#1#2#3#4#5#6{\if@noskipsec \leavevmode \fi
   \par \@tempskipa #5\relax
   \@afterindenttrue
   \ifdim \@tempskipa <\z@ \@tempskipa -\@tempskipa \fi
   \if@nobreak \everypar{}\else
     \addpenalty{\@secpenalty}\addvspace{\@tempskipa}\fi \@ifstar
{\@ssect{#3}{#4}{#5}{#6}}%
{\@dblarg{\@sect{#1}{#2}{#3}{#4}{#5}{#6}}}}

\newcount\@tempcntc
\def\@citex[#1]#2{\if@filesw\immediate\write\@auxout{\string\citation{#2}}\fi
  \@tempcnta\z@\@tempcntb\m@ne\def\@citea{}\@cite{\@for\@citeb:=#2\do
    {\@ifundefined                         
       {b@\@citeb}{\@citeo\@tempcntb\m@ne\@citea\def\@citea{,}{\bf ?}\@warning
       {Citation `\@citeb' on page \thepage \space undefined}}%
    {\setbox\z@\hbox{\global\@tempcntc0\csname b@\@citeb\endcsname\relax}%
     \ifnum\@tempcntc=\z@ \@citeo\@tempcntb\m@ne
       \@citea\def\@citea{,}\hbox{\csname b@\@citeb\endcsname}%
     \else
      \advance\@tempcntb\@ne
      \ifnum\@tempcntb=\@tempcntc
      \else\advance\@tempcntb\m@ne\@citeo
      \@tempcnta\@tempcntc\@tempcntb\@tempcntc\fi\fi}}\@citeo}{#1}}
\def\@citeo{\ifnum\@tempcnta>\@tempcntb\else\@citea\def\@citea{,}%
  \ifnum\@tempcnta=\@tempcntb\the\@tempcnta\else
   {\advance\@tempcnta\@ne\ifnum\@tempcnta=\@tempcntb \else \def\@citea{--}\fi
    \advance\@tempcnta\m@ne\the\@tempcnta\@citea\the\@tempcntb}\fi\fi}
\def\citenum#1{{\def\@cite##1##2{[##1]}\cite{#1}}}



        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%(Definition for ABSTRACT)                     %%
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%
%%the abstract is centred
\def\abstract{\if@twocolumn
\section*{Abstract}         %\bf has been removed
\else \small
\begin{center}
{\bf Abstract\vspace{-.5em}\vspace{0pt}}
\end{center}
\quotation
\fi}
\def\endabstract{\if@twocolumn\else\endquotation\fi}

\def\fnum@figure{{Fig. \thefigure}}

\def\fnum@table{{Table~\thetable}}

\long\def\@makecaption#1#2{
   \vskip 5pt 
   \setbox\@tempboxa\hbox{\parbox{0.8\linewidth}{\normalsize
         #1. #2 \vskip 8pt}}
   \ifdim \wd\@tempboxa >\hsize    %   IF longer than one line:
      \small #1. #2\par              %   THEN set as ordinary paragraph.
   \else                           %   ELSE  center.
      \hbox to\hsize{\hfil\box\@tempboxa\hfil}  
   \fi}

\newcommand\theHsubsubsubsection{\theHsubsection.\arabic{subsubsubsection}}
\newcommand\theHsubsubsubsubsection{\theHsubsection.\arabic{subsubsubsubsection}}

    \let\tocsubsubsubsection\HyPSD@tocsection
    \let\tocsubsubsubsubsection\HyPSD@tocsection

  \def\subsubsubsectionautorefname{subsubsubsection}%
  \def\subsubsubsubsectionautorefname{subsubsubsubsection}%
\def\toclevel@subsubsubsection{4}
\def\toclevel@subsubsubsubsection{5}

%\setcounter{secnumdepth}{5}
%\setcounter{tocdepth}{5}



% fundamentals
\usepackage{color}
%\usepackage{longtable} 
%To use for making tables same width as textwidth
\usepackage{tabularx}
\usepackage[table]{xcolor} %AH added [table] 1/24/14 to add color rows to tables. E.g. tbl 7.1
\usepackage{colortbl}
\usepackage{booktabs} % for use of \toprule, etc. per 
\usepackage{graphicx}
\usepackage{multirow}
\usepackage{ifthen}
\usepackage[T1]{fontenc}
\usepackage{lmodern}
\usepackage{setspace}

\usepackage{floatrow}
\floatsetup[table]{font=small,capposition=top}
\usepackage[format=plain,font=small,labelfont=bf,labelformat=simple,labelsep=colon]{caption}
\captionsetup{font={small,stretch=0.85,color=black!70}}


\usepackage{amssymb} 
\usepackage{xspace}
\usepackage{comment}
\usepackage{rotating}

\usepackage{selectp}

\usepackage{pdfpages}
\usepackage{authblk}
%\usepackage{sfmath} % AH added 1/7


%%% Some Default Styling

% reset itemize bullets
\renewcommand{\labelitemi}{$\bullet$}
\renewcommand{\labelitemi}{$\circ$}
% rename "Bibliography" to "References"
\renewcommand\bibname{References}

% Wrap any meta comments about things needing attention by
% document authors in a \fixme{...}.  These comments are hidden unless the
% lbnepaper/draft.tex is included.
\newcommand{\fixme}[1]{}

% The default "glossy" style uses TikZ which slows down the build but
% looks pretty.  If the \input{lbnepaper/draft} is set then this gets
% overridden to be "simple" which is fast but boring.
\newcommand{\lbnepagestyle}{glossy}
\newcommand{\lbnerefstyle}{references}
\newcommand{\lbneackstyle}{acknowledgement}

%Choose font family
%\fontencoding{T1}
% Common font families: 
% cmr (computer modern roman), 
% cmss (computer modern sans}, 
% cmtt (computer modern typewriter), 
% cmm (computer modern math italic), 
% cmsy (computer modern math symbolds), 
% cmex (computer modern math extensions), 
% ptm (Adobe Times), 
% phv (Adobe Helvetica), 
% pcr (Adobe Courier)
%\fontfamily{phv} 
%%m=Medium b=Bold bx=Bold extended sb=Semi-bold c=condensed
%\fontseries{m}
% n=normal it=italic sl=slanted sc=caps and small caps
%\fontshape{n}
%\fontsize{14}{10}
%\linespread{1}
%\selectfont 


\renewcommand{\rmdefault}{ptm}
\renewcommand{\seriesdefault}{m}  %AH trying to make font less bold-looking 1//14
\renewcommand{\normalsize}{\fontsize{12}{16}\selectfont}
