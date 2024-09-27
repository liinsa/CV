
\documentclass[lighthipster]{simplehipstercv}
% available options are: darkhipster, lighthipster, pastel, allblack, grey, verylight, withoutsidebar
% withoutsidebar
\usepackage[utf8]{inputenc}
\usepackage[default]{raleway}
\usepackage[margin=1cm, a4paper]{geometry}
\graphicspath{ {./img/} }

%------------------------------------------------------------------ Variablen

\newlength{\rightcolwidth}
\newlength{\leftcolwidth}
\setlength{\leftcolwidth}{0.23\textwidth}
\setlength{\rightcolwidth}{0.75\textwidth}

%------------------------------------------------------------------
\title{New Simple CV}
\author{\LaTeX{} Ninja}
\date{June 2019}

\pagestyle{empty}
\begin{document}


\thispagestyle{empty}
%-------------------------------------------------------------

\section*{Start}

\simpleheader{headercolour}{Salma}{Karim}{Ingénieur en Informatique}{white}



%------------------------------------------------

% this has to be here so the paracols starts..
\subsection*{}
\vspace{4em}

\setlength{\columnsep}{1.5cm}
\columnratio{0.23}[0.75]
\begin{paracol}{2}
\hbadness5000
%\backgroundcolor{c[1]}[rgb]{1,1,0.8} % cream yellow for column-1 %\backgroundcolor{g}[rgb]{0.8,1,1} % \backgroundcolor{l}[rgb]{0,0,0.7} % dark blue for left margin

\paracolbackgroundoptions

% 0.9,0.9,0.9 -- 0.8,0.8,0.8


\footnotesize
{\setasidefontcolour
\flushright
\begin{center}
    \roundpic{IMG_6740.jpeg}
\end{center}

\bg{cvgreen}{white}{About me}\\[0.5em]

{\footnotesize Je suis diplômée de Guangdong University of Technology en Chine, Je m'y connais en programmation: Language C, C++, Javascript,MySQL,MatLab,Cisco. Je m'adapte facilement, j'ai un grand sens de responsabilité et enthousiasme. Je peux être une valeur ajoutée à votre team par mes diverses qualités et mode te travaille. }

\bigskip

\bg{cvgreen}{white}{personal} \\[0.5em]
Salma KARIM

nationality: Marocaine 

1996

\bigskip

\bg{cvgreen}{white}{Areas of specialization} \\[0.5em]

Programmation  ~•~ Software ~•~ Data ~•~ AI

\bigskip



\bg{cvgreen}{white}{Interests}\\[0.5em]

\texttt{Technologie} ~/~ \texttt{Développement personnel } 

\texttt{Sports} ~/~ \texttt{Video Editing}

\texttt{L'art} ~/~ \texttt{Cuisine} ~/~ \texttt{les langues}
\vspace{4em}

\infobubble{\faAt}{cvgreen}{white}{salma95karim}\\{@gmail.com}

\infobubble{\faInstagram}{cvgreen}{white}{@Shiizumi}
\infobubble{\faFacebook}{cvgreen}{white}{KARIM Salma}
\infobubble{\faGithub}{cvgreen}{white}{Liinsa}

\phantom{turn the page}

\phantom{turn the page}
}
%-----------------------------------------------------------
\switchcolumn

\small




\section*{Éducation}
\begin{tabular}{r| p{0.5\textwidth} c}
    \cvevent{2020--2024}{Maîtrise en ingénierie informatique}{Guangdong University of Technology}{Guangzhou, Chine \color{cvred}}{Thèse: Design and implementation of AIoT based on packet tracer}{} \\
    \cvevent{2018--2020}{Diplôme de langue chinoise (HSK1, HSK2, HSK3)}{Institut Confucius Casablanca}{Université Hassan II \color{cvred}}{Acquisition des compétences linguistiques et culturelles en chinois mandarin.}{} \\
    \cvevent{2014--2018}{Licence en chimie analytique}{Université Hassan II}{Faculté des sciences Ain Chock, Casablanca \color{cvred}}{Thèse: Analyse des contaminants environnementaux dans les eaux potables à l’aide de techniques chromatographiques à Casablanca.}{} \\
    \cvevent{2012--2014}{Baccalauréat en sciences expérimentales (PC)}{Lycée Mohamed V}{Institut, Casablanca \color{cvred}}{Spécialité : Physique - Chimie.}{} \\
\end{tabular}






\section*{Expériences professionnelles}
\begin{tabular}{r| p{0.5\textwidth} c}
    \cvevent{2021--2024}{Professeur d'anglais}{Chine}{Guangdong, Guangzhou \color{cvred}}{
    \begin{itemize}
        \item Enseignement de l'anglais comme langue étrangère à divers niveaux.
        \item Cours sur les phonétiques pour améliorer la prononciation des élèves.
        \item Préparation et développement de leçons interactives et engageantes.
        \item Mise en œuvre de projets basés sur l'apprentissage phénoménal (PBL).
        \item Création de ressources didactiques DIY pour un apprentissage pratique.
    \end{itemize}
}{i2} \\
    \cvevent{2019--2021}{Professeur de français}{Bonjour Français}{Chine, Guangzhou \color{cvred}}{
    \begin{itemize}
        \item Enseignement du français comme langue étrangère à des élèves de divers niveaux.
        \item Formation sur la phonétique pour renforcer la prononciation.
        \item Préparation des étudiants aux examens TCF/TEF.
        \item Accompagnement dans la préparation à l’entretien de demande de visa français.
        \item Cours de français orientés vers les affaires.
    \end{itemize}
    }{vecteezy_france-flag-round-shape-png_22102503} 
    \\
    \cvevent{2022--2023}{Promotrice livestreamer sur TikTok}{Temps-partiel}{Chine, Guangzhou \color{cvred}}{
    \begin{itemize}
        \item Promotion de cristaux et pierres semi-précieuses via des ventes en direct sur TikTok.
        \item Analyse des données d'engagement et des tendances pour optimiser les stratégies de vente.
        \item Collaboration avec d'autres créateurs pour élargir la portée et améliorer l'expérience utilisateur.
    \end{itemize}
    }{}
\end{tabular}

\vspace{3em}
\section*{Compétences techniques \& informatiques}

\subsection*{Langages de programmation}
\begin{tabular}{l @{\hspace{0.5em}}l}
    \bg{skilllabelcolour}{iconcolour}{\texttt{C/C++}}  & Développement d'applications logicielles et systèmes embarqués. \\
    \bg{skilllabelcolour}{iconcolour}{\texttt{Python}} & Création d'outils d'analyse de données et de visualisation graphique. \\
    \bg{skilllabelcolour}{iconcolour}{\texttt{HTML/CSS, JavaScript}} & Conception et développement de sites web interactifs. \\
    \bg{skilllabelcolour}{iconcolour}{\texttt{SQL, JSON}} & Gestion de bases de données relationnelles et non relationnelles.
\end{tabular}

\subsection*{Systèmes embarqués et réseaux}
\begin{tabular}{l @{\hspace{0.5em}}l}
    \bg{skilllabelcolour}{iconcolour}{\texttt{Arduino, Raspberry Pi}}  & Prototypage et développement de solutions IoT. \\
    \bg{skilllabelcolour}{iconcolour}{\texttt{CISCO}} & Conception et gestion de systèmes réseau. \\
\end{tabular}

\subsection*{Frameworks et Outils}
\begin{tabular}{l @{\hspace{0.5em}}l}
    \bg{skilllabelcolour}{iconcolour}{\texttt{MatLab}} & Calculs numériques, modélisation et simulations. \\
    \bg{skilllabelcolour}{iconcolour}{\texttt{Matplotlib, NumPy, Pandas}} & Visualisation et manipulation de données. \\
\end{tabular}

\subsection*{Systèmes d'exploitation}
\begin{tabular}{l}
    GNU/Linux, Microsoft Windows, MacOS.
\end{tabular}

\vspace{3em}

\begin{minipage}[t]{0.3\textwidth}
\section*{Certificates \& Grants}
\begin{tabular}{>{\footnotesize\bfseries}r >{\footnotesize}p{0.55\textwidth}}
    2018 & Academic certificate from Henan university \\
    2019 & Certificate of completion, AEISEC Global Volunteer  
\end{tabular}
\bigskip


\end{minipage}\hfill
\begin{minipage}[t]{0.3\textwidth}

\section*{Languages}
\begin{tabular}{l | ll}
\textbf{Arabe} & C2 & {\phantom{x}\footnotesize Langue maternelle} \\
\textbf{Français } & C1 & \pictofraction{\faCircle}{cvgreen}{5}{black!30}{1}{\tiny} \\
\textbf{Anglais } & C1 & \pictofraction{\faCircle}{cvgreen}{5}{black!30}{1}{\tiny} \\
\textbf{Chinois} & B2 & \pictofraction{\faCircle}{cvgreen}{4}{black!30}{1}{\tiny}\\
\textbf{Allemand } & B1 & \pictofraction{\faCircle}{cvgreen}{2}{black!30}{3}{\tiny}
\end{tabular}
\bigskip
\end{minipage}



\vfill{} % Whitespace before final footer

%----------------------------------------------------------------------------------------
%	FINAL FOOTER
%----------------------------------------------------------------------------------------
\setlength{\parindent}{0pt}
\begin{minipage}[t]{\rightcolwidth}
\begin{center}\fontfamily{\sfdefault}\selectfont \color{black!70}
{\small Salma Karim \icon{\faEnvelopeO}{cvgreen}{} salma95karim@gmail.com \icon{\faMapMarker}{cvgreen}{} Casablanca \icon{\faPhone}{cvgreen}{} +212657545125 \newline\icon{\faAt}{cvgreen}{} \protect\url{https://github.com/liinsa}
}
\end{center}
\end{minipage}

\end{paracol}

\end{document}
