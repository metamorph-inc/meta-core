
(require '[dom-fun-eg.core :as eg])
(require '[clojure.data.json :as json])

(eg/echo "testing zmq connection" "arg 2")

(def j1 {:type :xxi :locate [0.0 0.0 0.0] :orient [1.0 0.0 0.0] :rotate 0.0})
(def j2 {:type :oof :locate [0.0 0.0 0.0] :orient [0.0 1.0 0.0] :rotate 0.0})
(def j3 {:type :oof :locate [0.0 0.0 0.0] :orient [1.0 0.0 0.0] :rotate 0.0})
(eg/infer-joint [ j1 j2 ])
(eg/infer-joint [ j1 j3 ])

(eg/infer-joint
 [ {:type :xxi :locate [1.0 2.0 0.0] :orient [1.0 2.0 3.0] :rotate 0.25}
   {:type :xxi :locate [1.0 1.0 0.0] :orient [4.0 5.0 6.0] :rotate 0.25} ])
