# -*- mode: make; -*-

sw_name=bxjsontools
sw_version=master
sw_report_label=xubuntu16.04
sw_report_version=1.0
sw_report_name=${sw_name}-${sw_version}_$(sw_report_label)_install
sw_tagged_report_name=${sw_name}-${sw_version}_$(sw_report_label)_install-$(sw_report_version)

.PHONY : all
all : html pdf

.PHONY : clean
clean :
	$(RM) $(sw_tagged_report_name).pdf
	$(RM) $(sw_report_name).html
	$(RM) $(sw_report_name).pdf
	$(RM) $(sw_report_name).aux
	$(RM) $(sw_report_name).log
	$(RM) $(sw_report_name).out
	$(RM) $(sw_report_name).tex
	$(RM) $(sw_report_name).toc
	find . -name "*~" -exec rm -f \{\} \;

.PHONY : tag
tag : final
	cp -f $(sw_tagged_report_name).pdf tagged/

.PHONY : final
final : $(sw_report_name).pdf
	cp -f $(sw_report_name).pdf $(sw_tagged_report_name).pdf

.PHONY : html
html : $(sw_report_name).html

.PHONY : pdf
pdf : $(sw_report_name).pdf

.PHONY : show
show : $(sw_report_name).html
	xdg-open $< &

.PHONY : show_pdf
show_pdf : $(sw_report_name).pdf
	xdg-open $< &

$(sw_report_name).html :  $(sw_report_name).rst
	rst2html $< > $@

$(sw_report_name).pdf : $(sw_report_name).tex
	pdflatex $<
	pdflatex $<

$(sw_report_name).tex :  $(sw_report_name).rst
	rst2latex --documentclass=article \
	  --documentoptions=a4paper,a4wide,10pt \
	  --use-latex-docinfo \
	  --use-verbatim-when-possible \
	  --stylesheet="style/geo" \
	  --use-latex-toc \
	  --syntax-highlight="long" \
	  --section-numbering \
	  $< > $@

# end
