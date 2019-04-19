PRJ = wsbf

BUILD = pdflatex ${EXTRA} ${PRJ} && (ls ${PRJ}.aux | xargs -n 1 bibtex) && pdflatex ${EXTRA} ${PRJ} && pdflatex ${EXTRA} ${PRJ}

.PHONY: all view clean

all : ${PRJ}.pdf

${PRJ}.pdf : *.tex *.bib
	${BUILD}

tarball :
	rm -f dledger.tar.gz
	tar -cvzf dledger.tar.gz *.tex figures/*.pdf *.sty *.bbl simulations

view : ${PRJ}.pdf
	open ${PRJ}.pdf &

clean:
	rm -Rf latex.out
	rm -f *.toc *.aux *.bbl *.blg *.log *~* *.bak *.out *synctex.gz *.ent ${PRJ}.pdf
