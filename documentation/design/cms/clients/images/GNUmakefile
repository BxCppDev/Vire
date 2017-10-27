

fig_files:=$(shell ls *.fig)
pdf_files:=$(fig_files:%.fig=%.pdf)
png_files:=$(fig_files:%.fig=%.png)

.PHONY : all
all : images


.PHONY : clean
clean :
	$(RM) *.pdf
	$(RM) *.png
	$(RM) *.fig.bak
	$(RM) *~

.PHONY : show_pdf
show_pdf : images_pdf
	xdg-open vire_domain_clients_gate_general.pdf &
	xdg-open vire_domain_client_system.pdf &
	xdg-open vire_domain_control.pdf &
	xdg-open vire_domain_monitoring.pdf &
	xdg-open vire_domain_subcontractor_system.pdf &

.PHONY : show_png
show_png : images_png
	xdg-open vire_domain_clients_gate_general.png &

.PHONY : images
images : images_pdf images_png

.PHONY : images_pdf
images_pdf : $(pdf_files)

%.pdf : %.fig
	@fig2dev -L pdf  -l landscape -b 3 -c $< ./$(notdir $(@))

.PHONY : images_png
images_png : $(png_files)

%.png : %.fig
	fig2dev -L png -m 3 -f tt -F -g white -b 3 -S 4 $< ./$(notdir $(@))

# end
