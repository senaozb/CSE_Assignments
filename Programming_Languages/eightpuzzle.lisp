; This program performs the game "Eight Puzzle" and the number of tiles is 3
; Level: Hard   

(defparameter arr (make-array '(3 3) :initial-element 1))
(defparameter input " ")
(defparameter stx -1)
(defparameter sty -1)
(defparameter check 0)
(defparameter flag 0)

(defun initBoard () ; Create a random array

   (defparameter arrGenerator (make-array '(9) :initial-element 0))
    
   (dotimes (n 3)
      append (dotimes (m 3)
                  (loop
                     (defparameter temp (random 9))
                     (defparameter randNum (+ 1 temp))
                     (defparameter value (aref arrGenerator temp))
                     (when (= value 0) (return 0))
                  )
                  (setf (aref arrGenerator (- randNum 1)) 1)   
                  (setf (aref arr n m) randNum)   
                  (if (= randNum 9)
                     (progn
                        (setq stx n)
                        (setq sty m)
                     )
                  )
            )
   )   
)

(defun printFunc () ; Print the board 
   (let ((k 0) (l 0))
      (dotimes (a 7)
         (setq i (- a 1))
         (dotimes (b 7)
            (setq j (- b 1))
            (cond ((or (= i -1) (= (mod i 2) 1))
                     (princ "*  "))
                     ((or (= j -1) (= (mod j 2) 1))
                     (princ "* "))
                     ((= (aref arr k l) 9)
                     (progn 
                        (princ "    ")
                        (incf l)
                     ))
                     ((/= (aref arr k l) 9)
                     (progn 
                        (format t " ~a  " (aref arr k l))
                        (incf l)
                     ))
            )
         )
         (format t "~%")
         (if (= (mod i 2) 0)
            (progn
               (incf k)
               (setf l 0)
            )
         )
      )
   )
)

(defun getInput () ; Get the input from the user
   (loop
      (princ "Please enter the coordinates as index of array and the direction as a char (e.g 00-r):")
      (setq input (read-line))
      (if (= (length input) 4)
         (progn
            (moveCheck)
            (if (= flag 1)
               (progn
                  (moveValid)
                  (if (= flag 0)
                     (write-line "This movement is not valid")
                  )
               )
            )
         )
         (write-line "Please enter a 4-character long input!")
      )
      (when (= flag 1) (return 1))
   )
)

(defun moveCheck () ; Check if the input is valid
   (setq x (char input 0))
   (setq y (char input 1))
   (setq d (char input 3))
   (setq flag 1)
         
   (cond ((or (alpha-char-p x) (alpha-char-p y))
         (progn 
            (write-line "These coordinates are not valid.")
            (setq flag 0)
         ))
         ((not (alpha-char-p d))
         (progn 
            (write-line "This direction is not valid.")
            (setq flag 0)
         ))
   )

   (if (= flag 1)
      (progn 
         (setq x (digit-char-p (char input 0)))
         (setq y (digit-char-p (char input 1)))

         (cond ((or (< x 0) (>= x 3) (< y 0) (>= y 3))
                (progn
                  (write-line "These coordinates are not valid.")
                  (setq flag 0)
                ))
                ((not (or (char= d #\r) (char= d #\l) (char= d #\u) (char= d #\d)))
                (progn
                  (write-line "This direction is not valid.")
                  (setq flag 0)
                ))
         )
      )
   )
)         
   
(defun moveValid () ; Check if the given movement is valid
   (setq x (digit-char-p (char input 0)))
   (setq y (digit-char-p (char input 1)))
   (setq d (char input 3))
   (setq flag 1)
        
   (cond ((char= d #\r)
          (progn 
            (if (not (and (= stx x) (/= y 2) (<= y sty)))
               (setq flag 0)
            )
          ))
         ((char= d #\l)
          (progn
            (if (not (and (= stx x) (/= y 0) (>= y sty)))
               (setq flag 0)
            )
          ))
         ((char= d #\u)
          (progn
            (if (not (and (= sty y) (/= x 0) (>= x stx)))
               (setq flag 0)
            )
          ))
         ((char= d #\d)
          (progn
            (if (not (and (= sty y) (/= x 2) (<= x stx)))
               (setq flag 0)
            )
          ))
   )
)

(defun move () ; Perform the movement
   (cond ((char= (char input 3) #\r)
          (moveRight))
         ((char= (char input 3) #\l)
          (moveLeft))
         ((char= (char input 3) #\u)
          (moveUp))
         ((char= (char input 3) #\d)
          (moveDown))
   )
)

(defun moveRight () ; Perform the right movement
   
   (setq X (digit-char-p (char input 0)))
   (setq Y (digit-char-p (char input 1)))
   (setq temp (aref arr stx sty))  
        
   (setq i sty)
   (loop 
      (setf (aref arr stx i) (aref arr stx (- i 1)))
      (decf i)
      (when (= i Y) (return 1))  
   )     

   (setf (aref arr X Y) temp)  
	
   (setq stx X)
   (setq sty Y)
)

(defun moveLeft () ; Perform the left movement

   (setq X (digit-char-p (char input 0)))
   (setq Y (digit-char-p (char input 1)))
   (setq temp (aref arr stx sty))  

   (setq i sty)
   (loop 
      (setf (aref arr stx i) (aref arr stx (+ i 1)))
      (incf i)
      (when (= i Y) (return 1))  
   )     

   (setf (aref arr X Y) temp)  
	
   (setq stx X)
   (setq sty Y)
)

(defun moveUp () ; Perform the up movement
   (setq X (digit-char-p (char input 0)))
   (setq Y (digit-char-p (char input 1)))
   (setq temp (aref arr stx sty))  

   (setq i stx)
   (loop 
      (setf (aref arr i sty) (aref arr (+ i 1) sty))
      (incf i)
      (when (= i X) (return 1))  
   )     

   (setf (aref arr X Y) temp)  
	
   (setq stx X)
   (setq sty Y)
)

(defun moveDown () ; Perform the down movement
   (setq X (digit-char-p (char input 0)))
   (setq Y (digit-char-p (char input 1)))
   (setq temp (aref arr stx sty)) 

   (setq i stx)
   (loop 
      (setf (aref arr i sty) (aref arr (- i 1) sty))
      (decf i)
      (when (= i X) (return 1))  
   )     

   (setf (aref arr X Y) temp)  
	
   (setq stx X)
   (setq sty Y) 
)

(defun checkFunc () ; Check if the game is finished
   (setf correctArr (make-array '(3 3) :initial-contents '((1 2 3) (4 5 6) (7 8 9))))
   (setq counter 0)

   (dotimes (i 3)
      (dotimes (j 3)
         (if (= (aref correctArr i j) (aref arr i j))
            (incf counter)
         )
      )
   )

   (if (= counter 9)
      (setq check 1)
   )
)

(defun play () ;Play the game
   (initBoard)

   (loop
      (printFunc)
      (getInput)
      (move)
      (checkFunc)
      (when (= check 1) (return 1))
   )

   (printFunc)
   (write-line "Game Over!")
)

(play) ; Call the play function to start the game