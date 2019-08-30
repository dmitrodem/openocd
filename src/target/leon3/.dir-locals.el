;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((c-mode
  (company-mode . t)
  (company-clang-arguments . ("-I../../.." "-I../../" "-I../../helper"))
  (company-backends . (company-clang))
  ))
