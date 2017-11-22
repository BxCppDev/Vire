=======================================
  Mapping support for SuperNEMO setup
=======================================

Contains
========

* *Vire Device Category* definition files (extension is ``.vdc``)
* *Vire Mapping Rule* definition files (extension is ``.vmr``)


Top setup category
==================
setup [0]


Description of device/port categories
=====================================

|-- module.dc [1000]
|   |   +-- module
|   |-- calorimeter.dc [1350]
|   |   `-- calorimeter_submodule.dc [1300]
|   |       |   +-- side
|   |       `-- calorimeter_optical_module.dc [1301]
|   |           |   +-- column
|   |           |   +-- row
|   |           `-- calorimeter_pmt.dc [1304]
|   |               |-- calorimeter_om_hv_input.pc [1399]
|   |               |-- calorimeter_om_signal_output.pc [1398]
|   |               `-- calorimeter_om_lifiber_input.pc [1397]
|   |                       +-- fiber
|   |-- tracker.dc [1201]
|   |   `-- tracker_submodule.dc [1200]
|   |       |   +-- side
|   |       |-- drift_cell.dc [1203]
|   |       |   |   +-- layer
|   |       |   |   +-- row
|   |       |   |-- drift_cell_anodic_wire.dc [1210]
|   |       |   |   `-- drift_cell_anode_signal_output.pc [1298]
|   |       |   ̀-- drift_cell_cathodic_ring.dc [1211]
|   |       |       |   +-- position
|   |       |       `-- drift_cell_cathode_signal_output.pc [1299]
|   |       |
|   |-- source_frame.dc [1500]
|   |-- coil.dc [1600]
|   |-- tent.dc [1700]
|   `-- shielding.dc [1800]
|-- cms.dc [10000]
|-- electronics.dc [20000]
|   |-- rack.dc [21000]
|   |   +-- rack
|   |   |-- crate.dc [22000]
|   |   |   +-- crate
