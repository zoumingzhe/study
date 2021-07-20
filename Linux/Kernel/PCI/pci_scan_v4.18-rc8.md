pci scan
========

+(.\arch\x86\pci\legacy.c:L59)pci_subsys_init
++(.\arch\x86\pci\legacy.c:L25)pci_legacy_init
+++(.\arch\x86\pci\common.c:L459)pcibios_scan_root
++++(.\drivers\pci\probe.c:L2968)pci_scan_root_bus
+++++(.\drivers\pci\probe.c:L2804)pci_create_root_bus
+++++(.\drivers\pci\probe.c:L2886)pci_bus_insert_busn_res
+++++(.\drivers\pci\probe.c:L2778)pci_scan_child_bus
++++++(.\drivers\pci\probe.c:L2637)pci_scan_child_bus_extend
+++++++(.\drivers\pci\probe.c:L2425)pci_scan_slot
++++++++(.\drivers\pci\probe.c:L2340)pci_scan_single_device
+++++++(.\drivers\pci\probe.c:L2340)pci_scan_single_device
+++++++(.\drivers\pci\probe.c:L1057)pci_scan_bridge_extend
++++++++(.\drivers\pci\search.c:L140)pci_find_bus
+++++++++(.\drivers\pci\search.c:L165)pci_find_next_bus
+++++++++(.\drivers\pci\search.c:L115)pci_do_find_bus
++++++++(.\drivers\pci\probe.c:L1005)pci_add_new_bus
++++++++(.\drivers\pci\probe.c:L2637)pci_scan_child_bus_extend
+++++(.\drivers\pci\probe.c:L2893)pci_bus_update_busn_res_end
++++(.\drivers\pci\bus.c:L343)pci_bus_add_devices
