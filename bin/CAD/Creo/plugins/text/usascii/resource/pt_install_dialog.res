!----------------------------------------------------------------------
!	File:		pt_install_dialog.res
!	Description:	resource file for Pro/TOOLKIT Install Test
!
!            PTC             File
!   Date   Version   Author  Vers  Comment
!--------- -------  -------- ----- ------------------------------------
!06-Apr-06 L-01-06    VSA     $$1  Created 
!----------------------------------------------------------------------

(Dialog pt_install_dialog
    (Components
        (PushButton                     CreateBtn)
        (PushButton                     CancelBtn)
        (Label                          Message)
		(Tree							CompTree)
    )

    (Resources
        (CreateBtn.Label                 "Reload list")
        (CreateBtn.TopOffset             4)
        (CreateBtn.BottomOffset          4)
        (CreateBtn.LeftOffset            4)
        (CreateBtn.RightOffset           4)
        (CancelBtn.Label                 "Cancel")
        (Message.TopOffset              4)
        (Message.BottomOffset           4)
        (Message.LeftOffset             4)
        (Message.RightOffset            4)
		(Message.Label					"")
		(CompTree.TreeDisplayRoot		True)
		(CompTree.ShowGrid				True)
		(CompTree.TreeLinkStyle			2)
		(CompTree.MinRows				16)
		(CompTree.Columns				32)
		(CompTree.TreeIndicateChildren  True)
		(CompTree..ScrollBarsWhenNeeded True)
        (.Label                         "Add Component")
        (.StartLocation                 5)
		(.DialogStyle                   3)
        (.Resizeable                    True)
        (.DefaultButton                 "CancelBtn")
        (.Layout
            (Grid (Rows 1 0 0) (Cols 1)
				CompTree
                Message
                (Grid (Rows 1) (Cols 1 1)
					CreateBtn
					CancelBtn
				)
            )
        )
    )
)



