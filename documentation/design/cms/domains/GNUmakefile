

rst_files:=$(shell ls *.rst)
html_files:=$(rst_files:%.rst=%.html)
pdf_files:=$(rst_files:%.rst=%.pdf)


.PHONY : all
all : doc

.PHONY : doc
doc : html pdf images

.PHONY : clean
clean : clean_images
	$(RM) vire_com_cms_domains.html
	$(RM) vire_com_cms_domain_mailbox_permissions.html
	$(RM) vire_com_cms_domains.pdf
	$(RM) vire_com_cms_domain_mailbox_permissions.pdf
	$(RM) *~

.PHONY : show_html
show_html : html
	@xdg-open vire_com_cms_domains.html &
	@xdg-open vire_com_cms_domain_mailbox_permissions.html &

.PHONY : show_pdf
show_pdf : pdf
	@xdg-open vire_com_cms_domains.pdf &
	@xdg-open vire_com_cms_domain_mailbox_permissions.pdf &

.PHONY : html
html : $(html_files)

%.html : %.rst images
	rst2html $< > $@

.PHONY : pdf
pdf : $(pdf_files)

%.pdf : %.rst images
	rst2pdf -s a4,serif $< -o $@

.PHONY : images
images :
	@cd images; make

.PHONY : clean_images
clean_images :
	@cd images; make clean

# end
