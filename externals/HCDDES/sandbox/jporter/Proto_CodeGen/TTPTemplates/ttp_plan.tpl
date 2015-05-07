TTA.Application_Command.run('File.New cluster')

TTA.Cluster.define ('{{APPNAME}}', byte_order = r'big_32_endian', tr_period = '1200 us', transmission_speed = r'2000', use_monitor_type = r'C2NF_mon', raw=1)

TTA.Cluster_Mode.define ('c_mode',  raw=1)
TTA.Cluster_Mode_of_Cluster.add ('c_mode', '{{APPNAME}}', raw=1)
TTA.Cluster_Mode_of_Cluster.add ('Startup_Mode', '{{APPNAME}}', raw=1)
TTA.Cluster_Mode_after_Cluster_Mode.add ('c_mode', 'Startup_Mode', raw=1)
TTA.Cluster_Mode_after_Cluster_Mode.link ('c_mode', 'Startup_Mode').set (request_mode_change = r'1 to -1', raw=1)

TTA.Msg_Type_P.define ('None', typedef = r'int', type_length = r'4', length = r'4',  type_cat = r'INT', raw=1)
TTA.Msg_Type_P.define ('Bit', typedef = r'char', type_length = r'1', length = r'1',  type_cat = r'INT', raw=1)
TTA.Msg_Type_P.define ('Byte', typedef = r'char', type_length = r'1', length = r'1',  type_cat = r'INT', raw=1)
TTA.Msg_Type_P.define ('HalfWord', typedef = r'short', type_length = r'2', length = r'2',  type_cat = r'INT', raw=1)
TTA.Msg_Type_P.define ('Word', typedef = r'int', type_length = r'4', length = r'4',  type_cat = r'INT', raw=1)
TTA.Msg_Type_P.define ('DoubleWord', typedef = r'long', type_length = r'4', length = r'4',  type_cat = r'INT', raw=1)

{{#HOST_SECTION}}
TTA.Host.define ('{{NODENAME}}', controller_type = r'TTTech_C2NF', serial_number = r'{{CPUNAME}}', raw=1)
TTA.Host_in_Cluster.add ('{{NODENAME}}', '{{APPNAME}}', raw=1)
TTA.Host_uses_Slot.add ('{{NODENAME}}', '{{NODENAME}}_slot', raw=1)
TTA.Host_in_Cluster_Mode.add ('{{NODENAME}}', 'c_mode', raw=1)
TTA.Host_in_Cluster_Mode.add ('{{NODENAME}}', 'Startup_Mode', raw=1)
TTA.Host_in_Cluster_Mode.link ('{{NODENAME}}', 'Startup_Mode').set (may_request_mode_changes = r'c_mode', raw=1)

{{#TASK_SECTION}}
TTA.Host_runs_Subsystem_in_Cluster_Mode.add ('{{NODENAME}}', '{{TASKNAME}}SubSystem', 'c_mode', raw=1)

{{#MSG_SECTION}}
{{#RECEIVEDMSG_SECTION}}
{{/RECEIVEDMSG_SECTION}}

{{#SENTMSG_SECTION}}
TTA.Message.define ('{{MSGNAME}}', init_value = r'0', raw=1)
TTA.Message_uses_Msg_Type.add ('{{MSGNAME}}', '{{MSGDATATYPE}}', raw=1)
TTA.Subsystem_sends_Message.add ('{{TASKNAME}}SubSystem', '{{MSGNAME}}', raw=1)
TTA.Cluster_Mode_uses_Message.add ('c_mode', '{{MSGNAME}}', raw=1)
TTA.Cluster_Mode_uses_Message.link ('c_mode', '{{MSGNAME}}').set (d_period = '{{MSGPERIOD}} ms', redundancy_degree = r'2', raw=1)
{{/SENTMSG_SECTION}}
{{/MSG_SECTION}}
{{/TASK_SECTION}}
{{/HOST_SECTION}}

TTA.Application_Command.run('Schedule.Make new schedule')
TTA.Application_Command.run('File.Save cluster')

