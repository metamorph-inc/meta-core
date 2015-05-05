(require '[dom-fun-eg.core :as eg])


(eg/init-shell "twoplates" "asm")
;;  m_ex_shell.set_name( file_stem );
;; 	m_ex_shell.activate_model( file_stem,
;; 			(file_extension == "asm" ? PRO_MDL_ASSEMBLY : PRO_MDL_PART ) );


(eg/create-shell "whatsit")
;;   if (! m_ex_shell.has_base_solid()) {
;;			m_log_cf.warnStream() << "no current model ";
;;			return false;
;;		}
;; 	  switch( status = m_ex_shell.create_shrinkwrap(name) ) {
;;			case PRO_TK_NO_ERROR: break;
;;			default:
;;				m_log_cf.warnStream() << "something went wrong " << status;
;;	  }

(eg/get-hydrostatic-result "0.001" "1123456" "0.0" "0.0")
;;    if (! m_ex_shell.has_wrapped_solid()) {
;;			m_log_cf.warnStream() << "no current wrapped model ";
;;			return false;
;;		}
;;		m_ex_shell.set_current_solid_to_wrapped();
;;    isis::hydrostatic::Result result;
;;		m_ex_shell.computeHydrostatic(result, true, tolerance, displaced, heel, trim);


(eg/set-displacement "2000")
(eg/get-hydrostatic-space)
