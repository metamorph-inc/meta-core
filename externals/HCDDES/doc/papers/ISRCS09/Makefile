ePDFLATEX = pdflatex
EPSTOPDF = ps2pdf
LATEX = latex
LATEXOPTS = -output-format dvi
BIBTEX = bibtex
DVIPDF = dvipdf
DVIPS = dvips

TARGETS = resilient.pdf

%.pdf : %.ps
	$(EPSTOPDF) $<
	$(EPSTOPDF) -dCompatibilityLevel=1.4 -dPDFSETTINGS=/prepress $@ $*_press.pdf
	cp $@ $*_`date +%h_%d_%Y_%I_%M_%p`.pdf

%.ps : %.dvi
	$(DVIPS) -Ppdf -f $< > $@

%.dvi : %.tex
	$(LATEX) $(LATEXOPTS) $*
	$(BIBTEX) $*
	$(LATEX) $(LATEXOPTS) $*
	$(LATEX) $(LATEXOPTS) $*

all: $(TARGETS)
	echo $(TARGETS)

clean:
	rm -f $(TARGETS) `basename $(TARGETS) .pdf`_*.pdf `basename $(TARGETS) .pdf`.ps *.log *.aux *out *.blg *.bbl *.dvi *.lof *.toc

