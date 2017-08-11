(Dialog comp_add_dialog
    (Components
        (PushButton                     OkBtn)
        (PushButton                     CancelBtn)
        (InputPanel                     Message)
		(Label                          EnterText)

    )

    (Resources
        (OkBtn.Label					"Ok")
        (OkBtn.TopOffset				4)
        (OkBtn.BottomOffset				4)
        (OkBtn.LeftOffset				4)
        (OkBtn.RightOffset				4)
        (CancelBtn.Label                "Cancel")
		(Message.Columns				30)
		(Message.Editable				True)
		(EnterText.Label				"Please enter the component name:")
        (.Label                         "Add Component")
        (.StartLocation                 5)
		(.DialogStyle                   3)
        (.Resizeable                    False)
        (.DefaultButton                 "OkBtn")
		(.Focus							"Message")
        (.Layout
            (Grid (Rows 1 1 0) (Cols 1)
				EnterText
                Message
                (Grid (Rows 1) (Cols 1 1)
					OkBtn
					CancelBtn
				)
            )
        )
    )
)
