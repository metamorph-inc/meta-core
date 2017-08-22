(Dialog connect_dialog
    (Components
        (List							LeftDatumList)
        (List							RightDatumList)
		(Label							LeftLabel)
		(Label							RightLabel)
		(PushButton						ConnectBtn)
    )

    (Resources
        (ConnectBtn.Label                 "Connect")
		(LeftLabel.Label                 "Datums:")
		(MidLabel.Label                 "Connectors:")
		(RightLabel.Label                 "Datums:")
        (.Label                         "Connect Components")
        (.StartLocation                 5)
		(.DialogStyle                   3)
        (.Resizeable                    True)
        (.DefaultButton                 "ConnectBtn")
        (.Layout
            (Grid (Rows 0 1 0) (Cols 1 1)
				LeftLabel
				RightLabel
				LeftDatumList
				RightDatumList
                (Grid (Rows 1) (Cols 1)
					ConnectBtn
				)
            )
        )
    )
)



