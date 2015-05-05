(import '[java.io BufferedReader InputStreamReader OutputStreamWriter])
(use 'clojure.contrib.server-socket)
(defn echo-server []
  (letfn [(echo [in out]
                    (binding [*in* (BufferedReader. (InputStreamReader. in))
                              *out* (OutputStreamWriter. out)]
                      (loop []
                        (let [input (read-line)]
                          (print input)
                          (flush))
                        (recur))))]
    (create-server 8080 echo)))

(def my-server (echo-server))
