(Dialog connector_select_dialog
    (Components
        (List							ConnectorList)
		(Label							SelectLabel)
		(PushButton						OkBtn)
    )

    (Resources
        (OkBtn.Label					"Ok")
		(SelectLabel.Label              "Select Connector")
        (.Label                         "Add to existing Connector")
        (.StartLocation                 5)
		(.DialogStyle                   3)
        (.Resizeable                    True)
        (.DefaultButton                 "OkBtn")
        (.Layout
            (Grid (Rows 0 1 0) (Cols 1)
				SelectLabel
				ConnectorList
				OkBtn
            )
        )
    )
)



