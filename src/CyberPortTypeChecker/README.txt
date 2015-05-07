ABOUT:
- A GME Interpreter for CyberComposition Meta-model under META tools @ ISIS, Vanderbilt University.
- Upon invoking the interpreter on a ModelicaComponent in a domain model, flattens(NOT changes) the underlying port hierarchy and displays it in the form of an Adjacency Grid.
- In future versions editing capabilities might be provided.

RESOURCES:
- GridCtrl.lib compiled from VS Project from site: http://www.codeproject.com/Articles/91607/CGridCtrl-with-Merge-Cell-and-Freeze-Row-Col-Capab

DESIGN NOTES:
- ModelHandler, PropEditor, UIGridCtrl are using C++ Notifier Listener/Observer Pattern.
- Notifier Listener type interactions({*} annotated not implemented): SYNTAX: /* {Notfier} ----[Info]----> {Observer} */
	- ModelHandler -----[New:Port/Connection, Modify:Port Type/Name]----> MatrixViewer
	- ModelHandler -----[New:PortType]----> PropEditor
	-* MatrixViewer -----[Delete:Connection]----> ModelHandler
	- MatrixViewer -----[Show:Port/Connection Info]----> PropEditor
	-* PropEditor -----[Modify:Port Type/Name]----> ModelHandler