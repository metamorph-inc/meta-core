(Dialog connector_datums_dialog
    (Components
        (PushButton                     OkBtn)
    )

    (Resources
        (OkBtn.Label					"Ok")
        (OkBtn.TopOffset				4)
        (OkBtn.BottomOffset				4)
        (OkBtn.LeftOffset				4)
        (OkBtn.RightOffset				4)
        (.Label                         "Create Datums")
        (.StartLocation                 5)
		(.DialogStyle                   3)
        (.Resizeable                    False)
        (.DefaultButton                 "OkBtn")
		(.Focus							"OkBtn")
        (.Layout
            (Grid (Rows 0) (Cols 1)
				OkBtn
            )
        )
    )
)
