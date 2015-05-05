(ns dom-fun-eg.core
  (:import [org.zeromq ZMQ ZMQ$Poller])
  (:require [clojure.edn :as edn])
  (:require [clojure.data.json :as json]) )



(def connection-name "tcp://127.0.0.1:15150")


(defn do-task [& value]
  (let [ctx (ZMQ/context 1)
        sock (.socket ctx ZMQ/DEALER)]
    (.connect sock connection-name)
    (.send sock "META" ZMQ/SNDMORE)
    (.send sock "0.0.2" ZMQ/SNDMORE)
    (doseq [item (butlast value)]
      (.send sock item ZMQ/SNDMORE) )
    (.send sock (last value))
    sock))


(defn echo [& message] (do-task (conj message "echo")))
(defn start-creo [] (do-task "start-creo" ))
(defn stop-creo [] (do-task "stop-creo" ))
(defn dump-all-features [] (do-task "dump-features" "all" ))


(defn dump-feature [featureName]
  (do-task "dump-feature" featureName))

(defn dump-selected-features []
  (do-task "dump-selected-feature"))

(defn dump-selected-feature-xml [filename]
  "prompt the user for a selection and then write
  it to the the file specified in the argument"
  (do-task "dump-selected-feature-xml" filename))

(defn create-shell [shell-name]
  "export the current assembly to the specified name (file)"
  (do-task "create-shell" shell-name))

(defn load-shell [shell-name]
  "export the current assembly to the specified file"
  (do-task "load-shell" shell-name))

(defn init-shell [name type]
  "set the working solid to the specified assembly (file)"
  (do-task "init-shell" name type))

(defn init-shell-current []
  "set the working solid to the current assembly"
  (do-task "init-shell"))

(defn activate-shell [shell-name]
  "export the current assembly to the specified file"
  (do-task "activate-shell" shell-name))

(defn disassemble [payload]
  "disassemble the current assembly"
  (do-task "disassemble" payload))


(defn get-hydrostatic-result  [tolerance depth heel trim]
  "This function obtains a result containing:
  - displaced volume : the volume of fluid displaced based on depth and angle.
  - wetted area : the area of the body in contact with the fluid."
  (do-task "get-hydrostatic-result" tolerance depth heel trim))


(defn get-hydrostatic-space []
  (do-task "get-hydrostatic-space"))

(defn set-displacement  [value]
  (do-task "set-displacement" value))

(defn set-trim  [value]
  (do-task "set-trim" value))

(defn set-heel  [value]
  (do-task "set-heel" value))

(defn denormalize-assembly []
  (do-task "denormalize-assembly"))

(defn infer-joint [joint-list]
  (apply do-task
         "infer-joint"
         (for [joint joint-list] (json/write-str joint) ) ) )

