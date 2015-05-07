
TTA.Application_Command.run ('File.New', '{{NODENAME}}', '..\\{{APPNAME}}.cdb')

{{#HOST_SECTION}}
TTA.Host.customize ('{{NODENAME}}', node_config = r'MPC555_AS8202NF', raw=1)
TTA.Node.App_Mode.define ('am', maximum_interrupt_latency = '150', raw=1)
TTA.Node.App_Mode_maps_to_Cluster_Mode.add('am', 'c_mode')

{{#TASK_SECTION}}
TTA.Node.App_Task.define ('{{TASKNAME}}_proc', time_budget = '{{WCEXECTIME}} us', raw=1)
TTA.Node.Subsystem_runs_Task.add ('{{TASKNAME}}SubSystem', '{{TASKNAME}}_proc', raw=1)
TTA.Node.Task_in_App_Mode.add ('{{TASKNAME}}_proc', 'am', raw=1)

{{#MSG_SECTION}}
{{#RECEIVEDMSG_SECTION}}
TTA.Node.Task_uses_Message.add ('{{TASKNAME}}_proc', {{MSGNAME}}access_type = r'agreed', raw=1)
TTA.Node.Task_uses_Message.link ('{{TASKNAME}}_proc', '{{MSGNAME}}').set (sends = r'no', receives = r'yes', raw=1)
{{/RECEIVEDMSG_SECTION}}

{{#SENTMSG_SECTION}}
TTA.Node.Task_uses_Message.add ('{{TASKNAME}}_proc', '{{MSGNAME}}', access_type = r'agreed', raw=1)
TTA.Node.Task_uses_Message.link ('{{TASKNAME}}_proc', '{{MSGNAME}}').set (sends = r'yes', receives = r'yes', raw=1)
{{/SENTMSG_SECTION}}

{{#IOMSG_SECTION}}
TTA.Node.IO_Message.define ('dummy_{{TASKNAME}}', raw=1)

TTA.Node.IO_Message.define ('dummy_{{TASKNAME}}', d_period = 'Cluster.tc_period / 2', window_size = 'Cluster.tc_period / 2', phase = 'Cluster.tc_period / 4', raw=1)
TTA.Node.Task_uses_Message.add ('{{TASKNAME}}_proc', 'dummy_{{TASKNAME}}', access_type = r'agreed', raw=1)
TTA.Node.Task_uses_Message.link ('{{TASKNAME}}_proc', 'dummy_{{TASKNAME}}').set (sends = r'yes', receives = r'no', raw=1)
{{/IOMSG_SECTION}}
{{/MSG_SECTION}}
{{/TASK_SECTION}}
{{/HOST_SECTION}}

TTA.Host.root.node_config.load_variant ('PN312_External')
TTA.Application_Command.run('Schedule.Make new schedule')
TTA.Application_Command.run('Schedule.Generate code')
TTA.Application_Command.run ('File.Save node')

