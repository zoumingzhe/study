pci scan
========

+(.\arch\x86\pci\legacy.c:L59)pci_subsys_init
++(.\arch\x86\pci\legacy.c:L25)pci_legacy_init
+++(.\arch\x86\pci\common.c:L459)pcibios_scan_root
++++(.\drivers\pci\search.c:L105)pci_find_next_bus
++++(.\arch\x86\pci\common.c:L681)pci_scan_bus_on_node
+++++(.\drivers\pci\probe.c:L1837)pci_scan_root_bus
++++++(.\drivers\pci\probe.c:L1672)pci_create_root_bus
++++++(.\drivers\pci\probe.c:L1774)pci_bus_insert_busn_res
++++++(.\drivers\pci\probe.c:L1610)pci_scan_child_bus
+++++++(.\drivers\pci\probe.c:L1428)pci_scan_slot
++++++++(.\drivers\pci\probe.c:L1335)pci_scan_single_device
+++++++(.\drivers\pci\probe.c:L723)pci_scan_bridge
++++++++(.\drivers\pci\search.c:L80)pci_find_bus
+++++++++(.\drivers\pci\search.c:L105)pci_find_next_bus
+++++++++(.\drivers\pci\search.c:L55)pci_do_find_bus
++++++++(.\drivers\pci\probe.c:L684)pci_add_new_bus
++++++++(.\drivers\pci\probe.c:L1610)pci_scan_child_bus
++++++(.\drivers\pci\probe.c:L1801)pci_bus_update_busn_res_end
++++++(.\drivers\pci\bus.c:L195)pci_bus_add_devices
