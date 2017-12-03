=====================================================
SuperNEMO Demonstrator Setup Device Model version 0.1
=====================================================

:Author: F.Mauger
:Date: 2017-03-28


.. code::

+-- setup (top level device model: "setup");
    +-- Demonstrator (device model: "snemo.demonstrator")
        +-- Detector (device model: "sndemo.detector")
	|   +-- Calorimeter (device model: "sndemo.calorimeter")
	|   |   +-- Wall[0..1] (device model: "sndemo.calo_main_wall")
	|   |       +-- OM[0..19][0..12] (device model: "sndemo.optical_module")
	|   |           +-- HVInput (port model: "sndemo.om_hv_input_pin")
	|   |           +-- LIFiber[0..1] (port model: "sndemo.om_fiber_input_pin")
	|   |           +-- Signal (port model: "sndemo.om_sig_ouput_pin")
	|   +-- Tracker (device model: "sndemo.tracker")
	|   |   +-- TrackingChamber[0..1] (device model: "sndemo.tracking_chamber")
	|   |       +-- Cell[0..8][0..112] (device model: "sndemo.tracker_drift_cell")
	|   |       |   +-- CathodeSignal[0..1] (port model: "sndemo.drift_cell_cathode_signal_pin")
	|   |       |   +-- AnodeSignalHV (port model: "sndemo.drift_cell_anode_signal_hv_pin")
	|   |       +-- GammaVeto[0..1][0..15] (device model: "sndemo.optical_module")
	|   |       +-- XWall[0..1] (device model: "sndemo.tracker_xwall")
	|   |           +-- OM[0..1][0..15] (device model: "sndemo.optical_module")
	|   +-- Coil (device model: "snemo.coil")
	|   +-- SourceFrame (device model: "sndemo.source_frame")
	|   +-- Electronics (device model: "sndemo.electronics")
	+-- Electronics (device model: "sndemo.electronics")
	|   TODO
	+-- Readout (device model: "sndemo.readout")
	|   TODO
        +-- CMS (device model: "sndemo.cms")
            +-- Coil (device model: "sndemo.cms.coil")
            |   +-- PS (device model: "sndemo.cms.coil")
            +-- Calorimeter (device model: "sndemo.cms.calorimeter")

..



..
