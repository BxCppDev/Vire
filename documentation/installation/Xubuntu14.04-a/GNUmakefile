# -*- mode: make; -*-

vr_report_label=xubuntu14.04-a
vr_report_version=0.1
vr_report_name=vire-trunk_$(vr_report_label)_report
vr_tagged_report_name=vire-trunk_$(vr_report_label)_report-$(vr_report_version)

.PHONY : all
all : html pdf

.PHONY : clean
clean :
	$(RM) $(vr_tagged_report_name).pdf
	$(RM) $(vr_report_name).html
	$(RM) $(vr_report_name).pdf
	$(RM) $(vr_report_name).aux
	$(RM) $(vr_report_name).log
	$(RM) $(vr_report_name).out
	$(RM) $(vr_report_name).tex
	$(RM) $(vr_report_name).toc
	find . -name "*~" -exec rm -f \{\} \;

.PHONY : tag
tag : final
	cp -f $(vr_tagged_report_name).pdf tagged/

.PHONY : final
final : $(vr_report_name).pdf
	cp -f $(vr_report_name).pdf $(vr_tagged_report_name).pdf

.PHONY : html
html : $(vr_report_name).html

.PHONY : pdf
pdf : $(vr_report_name).pdf

.PHONY : show
show : $(vr_report_name).html
	xdg-open $< &

.PHONY : show_pdf
show_pdf : $(vr_report_name).pdf
	xdg-open $< &

$(vr_report_name).html :  $(vr_report_name).rst
	rst2html $< > $@

$(vr_report_name).pdf : $(vr_report_name).tex
	pdflatex $<
	pdflatex $<

$(vr_report_name).tex :  $(vr_report_name).rst
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
