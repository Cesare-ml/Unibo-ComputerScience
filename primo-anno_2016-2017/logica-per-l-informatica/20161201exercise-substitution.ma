(**************************************************************************)
(*       ___	                                                          *)
(*      ||M||                                                             *)
(*      ||A||       A project by Andrea Asperti                           *)
(*      ||T||                                                             *)
(*      ||I||       Developers:                                           *)
(*      ||T||       A.Asperti, C.Sacerdoti Coen,                          *)
(*      ||A||       E.Tassi, S.Zacchiroli                                 *)
(*      \   /                                                             *)
(*       \ /        This file is distributed under the terms of the       *)
(*        v         GNU Lesser General Public License Version 2.1         *)
(*                                                                        *)
(**************************************************************************)

(* Esercizio 0 
   ===========
   
   Compilare i seguenti campi:
   
   Nome1: ...
   Cognome1: ...
   Matricola1: ...
   Account1: ...
   
   Nome2: ...
   Cognome2: ...
   Matricola2: ...
   Account2: ...

   Prima di abbandonare la postazione:

   * compilare il questionario in fondo al file
   
   * inviare via email il file a giulio.pellitta2@unibo.it
     usando il subject "laboratorio 27/10"
*)



(* ATTENZIONE
   ==========
   
   Non modificare le seguenti tre righe 
*)
include "nat/minus.ma".
definition max : nat → nat → nat ≝ λa,b:nat.let rec max n m on n ≝ match n with [ O ⇒ b | S n ⇒ match m with [ O ⇒ a | S m ⇒ max n m]] in max a b.
definition min : nat → nat → nat ≝ λa,b:nat.let rec min n m on n ≝ match n with [ O ⇒ a | S n ⇒ match m with [ O ⇒ b | S m ⇒ min n m]] in min a b.


(* Esercizio 1 
   ===========
   
   Definire il linguaggio delle formule riempiendo gli spazi 
*)
inductive Formula : Type ≝
| FBot: Formula
| FTop: Formula
| FAtom: nat → Formula
| FAnd: Formula → Formula → Formula
| FOr: Formula → Formula → Formula
| FImpl: Formula → Formula → Formula
| FNot: Formula → Formula
.


(* Esercizio 2 
   ===========

   Data la funzione di valutazione per gli atomi `v`, definire la 
   funzione `sem` per una generica formula `F` che vi associa la semantica
   (o denotazione) 
*)
let rec sem (v: nat → nat) (F: Formula) on F ≝
 match F with
  [ FBot ⇒ 0
  | FTop ⇒ 1
  | FAtom n ⇒ min(v n) 1 
  | FAnd F1 F2 ⇒ min(sem v F1) (sem v F2)
  | FOr F1 F2 ⇒ max (sem v F1) (sem v F2) 
  | FImpl F1 F2 ⇒ min(1 - (sem v F1)) (sem v F2)
  | FNot F1 ⇒ 1 - (sem v F1)
  ]
.

(* NOTA
   ====
   
   I comandi che seguono definiscono la seguente notazione:

   if e then risultato1 else risultato2
   
   Questa notazione permette di valutare l'espressione `e`. Se questa
   è vera restituisce `risultato1`, altrimenti restituisce `risultato2`.
   
   Un esempio di espressione è `eqb n m`, che confronta i due numeri naturali
   `n` ed `m`. 
   
   * [[ formula ]]v
   
   Questa notazione utilizza la funzione `sem` precedentemente definita, in 
   particolare `[[ f ]]v` è una abbreviazione per `sem v f`.


   ATTENZIONE
   ==========
   
   Non modificare le linee seguenti 
*)
definition if_then_else ≝ λT:Type.λe,t,f.match e return λ_.T with [ true ⇒ t | false ⇒ f].
notation > "'if' term 19 e 'then' term 19 t 'else' term 90 f" non associative with precedence 90 for @{ 'if_then_else $e $t $f }.
notation < "'if' \nbsp term 19 e \nbsp 'then' \nbsp term 19 t \nbsp 'else' \nbsp term 90 f \nbsp" non associative with precedence 90 for @{ 'if_then_else $e $t $f }.
interpretation "Formula if_then_else" 'if_then_else e t f = (if_then_else ? e t f).
notation < "[[ \nbsp term 19 a \nbsp ]] \nbsp \sub term 90 v" non associative with precedence 90 for @{ 'semantics $v $a }.
notation > "[[ term 19 a ]] \sub term 90 v" non associative with precedence 90 for @{ 'semantics $v $a }.
notation > "[[ term 19 a ]] term 90 v" non associative with precedence 90 for @{ sem $v $a }.
interpretation "Semantic of Formula" 'semantics v a = (sem v a).


(* Test 1
   ======
   
   Viene fornita una funzione di valutazione di esempio chiamata `v1101`. 
   Tale funzione associa agli atomi 0, 1 e 3 un valore pari a 1,
   invece a 2,4,5,6... un valore pari a 0. 
   
   Viene fornita una formula di esempio chiamata `esempio1` che rappresenta
   la formula 
    
       D => (C ∨ (B ∧ A))
       
   Dove A è rappresentato con l'atomo 0, B con l'atomo 1, ...
   
   Tale formula è valida per la funzione di valutazione `v1101`. 
   
   Il comando `eval normalize [[ esempio1 ]]v1101` permette di calcolare
   la funzione `sem` che avete appena definito. Tale funzione deve 
   computare a 1 (verrà mostrata una finestra col risultato).
   Se così non fosse significa che avete commesso un errore nella 
   definizione di `sem` e prima di continuare è necessario che la sistemiate.   
*)
definition v1101 ≝ λx.
      if eqb x 0 then 1  (* FAtom 0 ↦ 1 *)
 else if eqb x 1 then 1  (* FAtom 1 ↦ 1 *)
 else if eqb x 2 then 0  (* FAtom 2 ↦ 0 *)
 else if eqb x 3 then 1  (* FAtom 3 ↦ 1 *)
 else                 0. (* FAtom _ ↦ 0 *) 


definition esempio1 ≝ 
  (FImpl (FNot (FAtom 3)) (FOr (FAtom 2) (FAnd (FAtom 1) (FAtom 0)))).

(* Decommenta ed esegui. *)

 eval normalize on [[ esempio1 ]]v1101. 


(* Esercizio 3
   ===========
   
   Definire la funzione di sostituzione di una formula `G` al posto
   degli atomi uguali a `x` in una formula `F`. 
*)
let rec subst (x:nat) (G: Formula) (F: Formula) on F ≝
 match F with
  [ FBot ⇒ FBot
  | FTop ⇒ FTop
  | FAtom n ⇒ if (eqb n x) then (G) else (F)
  | FAnd F1 F2 ⇒ FAnd (subst x G F1) (subst x G F2)
  | FOr F1 F2 ⇒ FOr (subst x G F1) (subst x G F2)
  | FImpl F1 F2 ⇒ FImpl (subst x G F1) (subst x G F2)
  | FNot F ⇒ FNot (subst x G F)
  ].


(* NOTA
   ====
   
   I comandi che seguono definiscono la seguente notazione:

  * F [ G / x ]
  
  Questa notazione utilizza la funzione `subst` appena definita, in particolare
  la scrittura `F [ G /x ]` è una abbreviazione per `subst x G F`. 
  
  * F ≡ G
  
  Questa notazione è una abbreviazione per `∀v.[[ f ]]v = [[ g ]]v`. 
  Asserisce che for ogni funzione di valutazione `v`, la semantica di `f`
  in `v` è uguale alla semantica di `g` in `v`.


  ATTENZIONE
  ==========
  
  Non modificare le linee seguenti 
*)
notation < "t [ \nbsp term 19 a / term 19 b \nbsp ]" non associative with precedence 90 for @{ 'substitution $b $a $t }.
notation > "t [ term 90 a / term 90 b]" non associative with precedence 90 for @{ 'substitution $b $a $t }.
interpretation "Substitution for Formula" 'substitution b a t = (subst b a t).
definition equiv ≝ λF1,F2. ∀v.[[ F1 ]]v = [[ F2 ]]v.
notation "hvbox(a \nbsp break mstyle color #0000ff (≡) \nbsp b)"  non associative with precedence 45 for @{ 'equivF $a $b }.
notation > "a ≡ b" non associative with precedence 50 for @{ equiv $a $b }.
interpretation "equivalence for Formulas" 'equivF a b = (equiv a b).

(* Test 2
   ======
   
   Viene fornita una formula di esempio `esempio2`,
   e una formula `esempio3` che rimpiazzerà gli atomi
   `FAtom 2` di `esempio2`.
   
   Il risultato atteso è la formula:
   
        FAnd (FImpl (FOr (FAtom O) (FAtom 1)) (FAtom 1)) 
             (FOr (FAtom O) (FAtom 1))
   
*)

definition esempio2 ≝ (FAnd (FImpl (FAtom 2) (FAtom 1)) (FAtom 2)). 
   
definition esempio3 ≝ (FOr (FAtom 0) (FAtom 1)).

(* Decommenta ed esegui *)

 eval normalize on (esempio2 [ esempio3 / 2]). 



(* Esercizio 4
   ===========
   
   Dimostra il teorema di invarianza per sostituzione visto a lezione
*)
theorem sostituzione: ∀G1,G2,F,x. G1 ≡ G2 → F[G1/x] ≡ F[G2/x].
assume G1 : Formula.
assume G2 : Formula.
assume F : Formula.
assume x : nat.
suppose (G1 ≡ G2) (H).
we proceed by induction on F to prove (F[ G1/x ] ≡ F[ G2/x ]). 
case FBot.
  the thesis becomes (FBot[ G1/x ] ≡ FBot[ G2/x ]).
  the thesis becomes (FBot ≡ FBot[ G2/x ]).
  the thesis becomes (FBot ≡ FBot).
  the thesis becomes (∀v.[[FBot]]v = [[FBot]]v).
  assume v : (ℕ → ℕ).
  the thesis becomes (0 = [[FBot]]v).
  the thesis becomes (0 = 0).
  done.  
case FTop.
  the thesis becomes (FTop[ G1/x ] ≡ FTop[ G2/x ]).
  the thesis becomes (FTop ≡ FTop[ G2/x ]).
  the thesis becomes (FTop ≡ FTop).
  the thesis becomes (∀v.[[FTop]]v = [[FTop]]v).
  assume v : (ℕ → ℕ).
  the thesis becomes (1 = [[FTop]]v).
  the thesis becomes (1 = 1).
  done.
case FAtom.
  assume n : ℕ.
  the thesis becomes ((FAtom n)[ G1/x ] ≡ (FAtom n)[ G2/x ]).
  the thesis becomes 
    (if eqb n x then G1 else (FAtom n) ≡ (FAtom n)[ G2/x ]).    
  the thesis becomes
    (if eqb n x then G1 else (FAtom n) ≡
     if eqb n x then G2 else (FAtom n)).
  we proceed by cases on (eqb n x) to prove 
    (if eqb n x then G1 else (FAtom n) ≡
     if eqb n x then G2 else (FAtom n)).
  case true.
    the thesis becomes (G1 ≡ G2).
    by H done.
  case false.
    the thesis becomes (FAtom n ≡ FAtom n). 
    the thesis becomes (∀v.[[ FAtom n ]]v = [[ FAtom n ]]v). 
    assume v : (ℕ → ℕ).
    the thesis becomes ([[ FAtom n ]]v = [[ FAtom n ]]v). 
    done.
case FAnd.
  assume F1 : Formula.
  by induction hypothesis we know (F1[ G1/x ] ≡ F1[ G2/x ]) (IH1).    
  assume F2 : Formula.
  by induction hypothesis we know (F2[ G1/x ] ≡ F2[ G2/x ]) (IH2).    
  the thesis becomes 
    (∀v.[[ (FAnd F1 F2)[ G1/x ] ]]v = [[ (FAnd F1 F2)[ G2/x ] ]]v).
  assume v : (ℕ → ℕ). 
  the thesis becomes 
    (min ([[ F1[ G1/x ] ]]v) ([[ F2[ G1/x ] ]]v) = 
     min ([[ F1[ G2/x ] ]]v) ([[ F2[ G2/x ] ]]v)).
  by IH1 we proved (∀v1.[[ F1[ G1/x ] ]]v1 = [[ F1[ G2/x ] ]]v1) (IH11).
  by IH2 we proved (∀v2.[[ F2[ G1/x ] ]]v2 = [[ F2[ G2/x ] ]]v2) (IH22).
  by IH11 we proved ([[ F1[ G1/x ] ]]v = [[ F1[ G2/x ] ]]v) (IH111).
  by IH22 we proved ([[ F2[ G1/x ] ]]v = [[ F2[ G2/x ] ]]v) (IH222).
  conclude 
      (min ([[ F1[ G1/x ] ]]v) ([[ F2[ G1/x ] ]]v)) 
    = (min ([[ F1[ G2/x ] ]]v) ([[ F2[ G1/x ] ]]v)) by IH111.
    = (min ([[(F1[ G2/x ])]]v) ([[(F2[ G2/x ])]]v)) by IH222.
  (*END*)
  done.
case FOr.
  assume F1 : Formula.
  by induction hypothesis we know (F1[ G1/x ] ≡ F1[ G2/x ]) (IH1).
  assume F2 : Formula.
  by induction hypothesis we know (F2[ G1/x ] ≡ F2[ G2/x ]) (IH2).
  the thesis becomes
    (∀v.[[ (FOr F1 F2)[ G1/x ] ]]v = [[ (FOr F1 F2)[ G2/x ] ]]v).
    assume v : (ℕ → ℕ).
    the thesis becomes
      (max ([[ F1[ G1/x ] ]]v) ([[ F2[ G1/x ] ]]v) = 
      max ([[ F1[ G2/x ] ]]v) ([[ F2[ G2/x ] ]]v)
      ).
  by IH1 we proved (∀v1.[[ F1[ G1/x ] ]]v1 = [[ F1[ G2/x ] ]]v1) (IH11).
  by IH2 we proved (∀v1.[[ F2[ G1/x ] ]]v1 = [[ F2[ G2/x ] ]]v1) (IH22).
  by IH11 we proved ([[ F1[ G1/x ] ]]v = [[ F1[ G2/x ] ]]v) (IH111).
  by IH22 we proved ([[ F2[ G1/x ] ]]v = [[ F2[ G2/x ] ]]v) (IH222).
  conclude
      (max ([[ F1[ G1/x ] ]]v) ([[ F2[ G1/x ] ]]v)) 
    = (max ([[ F1[ G2/x ] ]]v) ([[ F2[ G1/x ] ]]v)) by IH111.
    = (max ([[(F1[ G2/x ])]]v) ([[(F2[ G2/x ])]]v)) by IH222.
  
  done.
case FImpl.
  assume F1 : Formula.
  by induction hypothesis we know (F1[ G1/x ] ≡ F1[ G2/x ]) (IH1).
  assume F2 : Formula.
  by induction hypothesis we know (F2[ G1/x ] ≡ F2[ G2/x ]) (IH2).
  the thesis becomes
    (∀v.[[ (FImpl F1 F2)[ G1/x ] ]]v = [[ (FImpl F1 F2)[ G2/x ] ]]v).
    assume v : (ℕ → ℕ).
    the thesis becomes
      (min (1 - [[ F1[ G1/x ] ]]v) ([[ F2[ G1/x ] ]]v) = 
      min (1 - [[ F1[ G2/x ] ]]v) ([[ F2[ G2/x ] ]]v)
      ).
  by IH1 we proved (∀v1.[[ F1[ G1/x ] ]]v1 = [[ F1[ G2/x ] ]]v1) (IH11).
  by IH2 we proved (∀v1.[[ F2[ G1/x ] ]]v1 = [[ F2[ G2/x ] ]]v1) (IH22).
  by IH11 we proved ([[ F1[ G1/x ] ]]v = [[ F1[ G2/x ] ]]v) (IH111).
  by IH22 we proved ([[ F2[ G1/x ] ]]v = [[ F2[ G2/x ] ]]v) (IH222).
  conclude
      (min (1 - [[ F1[ G1/x ] ]]v) ([[ F2[ G1/x ] ]]v)) 
    = (min (1 - [[ F1[ G2/x ] ]]v) ([[ F2[ G1/x ] ]]v)) by IH111.
    = (min (1 - [[(F1[ G2/x ])]]v) ([[(F2[ G2/x ])]]v)) by IH222.
  
  done.
case FNot.
  assume F : Formula.
  by induction hypothesis we know (F[ G1/x ] ≡ F[ G2/x ]) (IH).
  the thesis becomes
    (∀v.[[ (FNot F)[ G1/x ] ]]v = [[ (FNot F)[ G2/x ] ]]v).
    assume v : (ℕ → ℕ).
    the thesis becomes
      (1 - [[ F[ G1/x ] ]]v = 1 - [[ F[ G2/x ] ]]v).
  by IH we proved (∀v1.[[ F[ G1/x ] ]]v1 = [[ F[ G2/x ] ]]v1) (IH1).
  by IH1 we proved ([[ F[ G1/x ] ]]v = [[ F[ G2/x ] ]]v) (IH11).
  conclude
      (1 - [[ F[ G1/x ] ]]v)
    = (1 - [[ F[ G2/x ] ]]v) by IH11.
  done.
qed.
