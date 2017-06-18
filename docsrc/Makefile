LATEX = pdflatex

FLAGS = -halt-on-error

all: lat2eps.pdf liblat2eps.pdf

clean:
	rm -f *.aux *.log *.out *.dvi

cleaner:
	rm -f lat2eps.pdf liblat2eps.pdf *.aux *.log *.out *.dvi

lat2eps.pdf:	lat2eps.tex
	$(LATEX) $(FLAGS) lat2eps.tex
	$(LATEX) $(FLAGS) lat2eps.tex

liblat2eps.pdf:	liblat2eps.tex
	$(LATEX) $(FLAGS) liblat2eps.tex
	$(LATEX) $(FLAGS) liblat2eps.tex

