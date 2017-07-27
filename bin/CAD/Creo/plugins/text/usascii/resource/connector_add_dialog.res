(Dialog connector_add_dialog
    (Components
        (PushButton                     CreateNewBtn)
        (PushButton                     UseExistingBtn)
		(Label                          Name)
		(TextArea                       NewConnectorName)
    )

    (Resources
        (CreateNewBtn.Label              "Create New")
        (UseExistingBtn.Label            "Use Existing")
		(NewConnectorName.Rows					1)
		(NewConnectorName.Columns				30)
		(Name.Label						"New Connector Name:")
        (.Label                         "Add Connector")
        (.StartLocation                 5)
		(.DialogStyle                   3)
        (.Resizeable                    False)
		(.Focus							"NewConnectorName")
        (.Layout
            (Grid (Rows 0 0 0) (Cols 0)
				Name
				NewConnectorName
				(Grid (Rows 0) (Cols 1 1)
					CreateNewBtn
					UseExistingBtn
				)
            )
        )
    )
)
