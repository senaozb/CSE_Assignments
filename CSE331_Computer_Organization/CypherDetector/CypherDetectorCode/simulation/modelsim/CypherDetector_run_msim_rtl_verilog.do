transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/Okul/Homework/331/BonusHW {D:/Okul/Homework/331/BonusHW/CypherDetector.v}
vlog -vlog01compat -work work +incdir+D:/Okul/Homework/331/BonusHW {D:/Okul/Homework/331/BonusHW/control.v}
vlog -vlog01compat -work work +incdir+D:/Okul/Homework/331/BonusHW {D:/Okul/Homework/331/BonusHW/datapath.v}
vlog -vlog01compat -work work +incdir+D:/Okul/Homework/331/BonusHW {D:/Okul/Homework/331/BonusHW/CypherDetectorTB.v}

