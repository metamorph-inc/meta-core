c#######################################################################
c#  Copyright (C) 2006 by Marek Wojciechowski
c#  <mwojc@p.lodz.pl>
c#
c#  Distributed under the terms of the GNU General Public License (GPL)
c#  http://www.gnu.org/copyleft/gpl.html
c#######################################################################
c
cc
ccc
cccc
ccccc BASIC FFNN PROPAGATION ROUTINES
cccc
ccc
cc
c
c************************************************************************
      subroutine prop(x, conec, n, units, u)
c************************************************************************
c
c.....Gets conec and units with input already set 
c.....and calculates all activations.
c.....Identity input and sigmoid activation function for other units
c.....is assumed
c
      implicit none
c.....variables
      integer n, u, conec(n,2)
      double precision x(n), units(u)
c.....helper variables
      integer src, trg, ctrg, xn
c.....f2py statements
cf2py intent(in, out) units
      
c.....propagate signals with sigmoid activation function
      if (n.gt.0) then
          ctrg = conec(1,2)
          units(ctrg) = 0.
          do xn=1,n
              src = conec(xn,1)
              trg = conec(xn,2)
              !if next unit
              if (trg.ne.ctrg) then
                  units(ctrg) = 1./(1.+exp(-units(ctrg)))
                  ctrg = trg
                  if (src.eq.0) then !handle bias
                      units(ctrg) = x(xn)
                  else
                      units(ctrg) = units(src) * x(xn)
                  endif
              else
                  if (src.eq.0) then !handle bias
                      units(ctrg) = units(ctrg) + x(xn)
                  else
                      units(ctrg) = units(ctrg) + units(src) * x(xn)
                  endif
              endif
          enddo
          units(ctrg) = 1./(1.+exp(-units(ctrg))) !for last unit
      endif

      return
      end
      
c************************************************************************
      subroutine sqerror(x, conec, n, units, u, inno, i, outno, o, 
     &                   Input, Targ, p, sqerr)
c************************************************************************
c
c.....Takes Input and Target patterns and returns sum of squared errors
c
      implicit none
c.....variables
      integer n, u, i, o, p, conec(n,2), inno(i), outno(o)
      double precision x(n), units(u), Input(p,i), Targ(p,o), sqerr
c.....helper variables
      integer k, pat
c.....f2py statements
cf2py intent(out) sqerr

      sqerr = 0.
c.....loop over given patterns
      DO pat = 1,p
c.........set input units
          do k=1,i
             units(inno(k)) = Input(pat,k)
          enddo
c.........propagate signals
          call prop(x, conec, n, units, u)
c.........sum squared errors
          do k=1,o
             sqerr = sqerr + (units(outno(k)) - Targ(pat,k))**2
          enddo  
      ENDDO
      sqerr = 0.5d0*sqerr
      
      return
      end    
      
c************************************************************************
      subroutine grad(x, conec, n, bconecno, bn, units, u, inno, i,  
     &                outno, o, Input, Targ, p, xprime)
c************************************************************************
c
c.....Takes conec, bconecno, Input and Target patterns and returns 
c.....gradient calculated with error backpropagation
c
      implicit none
c.....variables
      integer n, bn, u, i, o, p 
      integer conec(n,2), bconecno(bn), inno(i), outno(o)
      double precision x(n), units(u), Input(p,i), Targ(p,o)
      double precision xprime(n), diff(o), bunits(u)
c.....helper variables
      integer k, pat, src, trg, ctrg
      double precision deriv, cx
c.....f2py statements
cf2py intent(out) xprime

c.....initialize xprime
      do k=1,n
          xprime(k) = 0.
      enddo
c.....loop over given patterns
      DO pat = 1,p
c.........propagate input signals
          do k=1,i
              units(inno(k)) = Input(pat,k)
          enddo
          call prop(x, conec, n, units, u)        
c.........set diffs at network output as back network inputs
          do k=1,o
	         diff(k) = units(outno(k)) - Targ(pat,k)
	         deriv = units(outno(k)) * (1. - units(outno(k))) !ugly
	         bunits(outno(k)) = diff(k) * deriv
          enddo
c.........backpropagate errors
          if (bn.gt.0) then
              ctrg = conec(bconecno(1),1)
              bunits(ctrg) = 0.
              do k=1,bn
                  src = conec(bconecno(k),2)
                  trg = conec(bconecno(k),1)
	              cx = x(bconecno(k))
                  if (trg.ne.ctrg) then  !if next unit
	                  deriv = units(ctrg) * (1. - units(ctrg)) !ugly 
				      bunits(ctrg) = bunits(ctrg) * deriv
                      ctrg = trg
                      bunits(ctrg) = bunits(src) * cx
	              else
	                  bunits(ctrg) = bunits(ctrg) + bunits(src) * cx
	              endif
              enddo
              deriv = units(ctrg) * (1 - units(ctrg))
              bunits(ctrg) = bunits(ctrg) * deriv  !for last unit
          endif
c.........add gradient elements to overall xprime
          do k=1,n
              src = conec(k,1)
              trg = conec(k,2)
              if (src.eq.0) then !handle bias
                  xprime(k) = xprime(k) + bunits(trg)
              else
                  xprime(k) = xprime(k) + units(src) * bunits(trg)
              endif
          enddo
      ENDDO
      
      return
      end    

c************************************************************************
      subroutine recall(x, conec, n, units, u, inno, i, outno, o, 
     &                  input, output)
c************************************************************************
c
c.....Takes single input pattern and returns network output
c
      implicit none
c.....variables
      integer n, u, i, o, conec(n,2), inno(i), outno(o)
      double precision x(n), units(u), input(i), output(o)
c.....helper variables
      integer k
c.....f2py statements
cf2py intent(out) output

c.....set input units
      do k=1,i
         units(inno(k)) = input(k)
      enddo
c.....propagate signals
      call prop(x, conec, n, units, u)
c.....get output
      do k=1,o
         output(k) = units(outno(k))
      enddo  
      
      return
      end

c************************************************************************
      subroutine diff(x, conec, n, dconecno, dn, dconecmk, units, u,
     &                inno, i, outno, o, input, deriv)
c************************************************************************
c
c.....Takes single input pattern and returns network partial derivatives
c.....in the form d(output,o)/d(input,i). 'units' contain now activation
c.....derivatives
c
      implicit none
c.....variables
      integer n, dn, u, i, o, conec(n,2), dconecno(dn), dconecmk(i+1)
      integer inno(i), outno(o)
      double precision x(n), units(u), dunits(u), input(i), deriv(o,i)
c.....helper variables
      integer k, di, trg, src, ctrg, xn
      double precision dx
c.....f2py statements
cf2py intent(out) deriv

c.....first set inputs for usual and derivative network units
      do k=1,i
          units(inno(k)) = input(k)
          dunits(inno(k)) = 0d0
      enddo
c.....calculate derivatives of activation functions --> units became
c.....units derivatives (ugly, usable only for sigmoid function 
c.....and identity input)
      call prop(x, conec, n, units, u)
      do k=1,u
	      units(k) = units(k) * (1d0 - units(k))
      enddo
      do k=1,i
         units(inno(k)) = 1d0
      enddo
c.....prepare output units for derivative network
      do k=1,o
          dunits(outno(k)) = 0d0
      enddo
c.....loop over inputs
      do di=1,i
c.........set current input unit derivative as network input
	      dunits(inno(di)) = units(inno(di))
c.........propagate signals through derivative network (dunits became 
c.........net units and units derivatives are now scaling factors)
          if (dn.gt.0) then
              ctrg = conec(dconecno(dconecmk(di)+1),2)
              dunits(ctrg) = 0.
              do xn=dconecmk(di)+1,dconecmk(di+1)
                  src = conec(dconecno(xn),1)
                  trg = conec(dconecno(xn),2)
                  dx = x(dconecno(xn))
                  if (trg.ne.ctrg) then
                      dunits(ctrg) = dunits(ctrg) * units(ctrg)
                      ctrg = trg
                      dunits(ctrg) = dunits(src) * dx
                  else
                      dunits(ctrg) = dunits(ctrg) + dunits(src) * dx
                  endif
              enddo
              dunits(ctrg) = dunits(ctrg) * units(ctrg) !for last unit
          endif
c.........save network outputs (do/di)
          do k=1,o
		      deriv(k, di) = dunits(outno(k))
              dunits(outno(k)) = 0d0
          enddo
c.........restore current input
          dunits(inno(di)) = 0d0 
      enddo

      RETURN
      end
c
cc
ccc
cccc
ccccc EXTENSIONS OF BASIC ROUTINES 
cccc
ccc
cc
c
c************************************************************************
      subroutine func(x, conec, n, bconecno, bn, units, u, inno, i,  
     &                outno, o, Input, Targ, p, sqerr)
c************************************************************************
c
c.....Just calls sqerror, but now the agruments list
c.....is compatibile with grad. This compatibility is needed by scipy 
c.....optimizers.
c
      implicit none
c.....variables
      integer n, bn, u, i, o, p 
      integer conec(n,2), bconecno(bn), inno(i), outno(o)
      double precision x(n), units(u), Input(p,i), Targ(p,o), sqerr
c.....f2py statements
cf2py intent(out) sqerr

      call sqerror(x, conec, n, units, u, inno, i, outno, o, 
     &             Input, Targ, p, sqerr)
     
      return
      end

c************************************************************************
      subroutine pikaiaff(x, ffn, conec, n, units, u, inno, i, outno, o, 
     &                    Input, Targ, p, bound1, bound2, isqerr)
c************************************************************************
c
c.....Routine for use with pikaia - genetic algorithm based optimizer.
c.....Takes Input and Target patterns and returns inverse of
c.....sum of quared errors. Note: (bound1, bound2)
c.....is constraint range for x.
c
      implicit none
c.....variables
      integer n, ffn, u, i, o, p, conec(n,2), inno(i), outno(o)
      double precision x(n), x2(n), units(u), Input(p,i), Targ(p,o)
      double precision bound1, bound2, isqerr
c.....f2py statements
cf2py intent(out) isqerr
      
c.....first map x vector values from 0,1 to bound1,bound2
      call vmapa(x, n, 0d0, 1d0, bound1, bound2, x2)
c.....now propagate patterns and obtain error
      call sqerror(x2, conec, n, units, u, inno, i, outno, o, 
     &             Input, Targ, p, isqerr)
c.....inverse error
      isqerr = 1. / isqerr
      
      RETURN
      end

c************************************************************************
      subroutine normcall(x, conec, n, units, u, inno, i, outno, o,
     &                    eni, deo, input, output)
c************************************************************************
c
c.....Takes single input pattern and returns network output
c.....This have the same functionality as recall but now input and
c.....output are normalized inside the function.
c.....eni = [ai, bi], eno = [ao, bo] - parameters of linear mapping
c
      implicit none
c.....variables
      integer n, u, i, o, conec(n,2), inno(i), outno(o)
      double precision x(n), units(u), input(i), output(o)
      double precision eni(i,2), deo(o,2)
c.....f2py statements
cf2py intent(out) output, istat

c.....set input units
      call setin(input, inno, i, eni, units, u)
c.....propagate signals
      call prop(x, conec, n, units, u)
c.....get output
      call getout(units, u, outno, o, deo, output)
      
      return
      end
c
c************************************************************************
      subroutine normdiff(x, conec, n, dconecno, dn, dconecmk, units,
     &                    u, inno, i, outno, o, eni, ded, input, deriv)
c************************************************************************
c
c.....Takes single input pattern and returns network partial derivatives
c.....in the form d(output,o)/d(input,i). 'units' contain now activation
c.....derivatives
c.....This have the same functionality as diff but now input and
c.....output are normalized inside function
c
c.....Solution not very smart, whole diff routine is rewritten here...
c
      implicit none
c.....variables
      integer n, dn, u, i, o, conec(n,2), dconecno(dn), dconecmk(i+1)
      integer inno(i), outno(o)
      double precision x(n), units(u), dunits(u), input(i), deriv(o,i)
      double precision eni(i,2), ded(o,i)
c.....helper variables
      integer k, di, trg, src, ctrg, xn
      double precision dx
c.....f2py statements
cf2py intent(out) deriv

c.....first set inputs for usual and derivative network units
      call setin(input, inno, i, eni, units, u)
c.....propagate through network
      call prop(x, conec, n, units, u)
c.....calculate derivatives of activation functions --> units became
c.....units derivatives (ugly, usable only for sigmoid function 
c.....and identity input)
      do k=1,u
	      units(k) = units(k) * (1d0 - units(k))
          !units(k) = units(k) * (1d0 - units(k)) * (1d0-2d0*units(k))
      enddo
      do k=1,i
          units(inno(k)) = 1d0
      enddo
c.....prepare output units and scaling factors for derivative network
      do k=1,o
          dunits(outno(k)) = 0d0
      enddo
c.....loop over inputs
      do di=1,i
c.........set current input unit derivative as network input
	      dunits(inno(di)) = units(inno(di))
c.........propagate signals through derivative network (dunits became 
c.........net units and units derivatives are now scaling factors)
          if (dn.gt.0) then
              ctrg = conec(dconecno(dconecmk(di)+1),2)
              dunits(ctrg) = 0.
              do xn=dconecmk(di)+1,dconecmk(di+1)
                  src = conec(dconecno(xn),1)
                  trg = conec(dconecno(xn),2)
                  dx = x(dconecno(xn))
                  if (trg.ne.ctrg) then
                      dunits(ctrg) = dunits(ctrg) * units(ctrg)
                      ctrg = trg
                      dunits(ctrg) = dunits(src) * dx
                  else
                      dunits(ctrg) = dunits(ctrg) + dunits(src) * dx
                  endif
              enddo
              dunits(ctrg) = dunits(ctrg) * units(ctrg) !for last unit
          endif
c.........save network outputs (do/di)
          do k=1,o
		      deriv(k, di) = dunits(outno(k))*ded(k,di)
              dunits(outno(k)) = 0d0
          enddo
c.........restore current input
          dunits(inno(di)) = 0d0 
      enddo

      RETURN
      end
c
c************************************************************************
      subroutine normcall2(x, conec, n, units, u, inno, i, outno, o,
     &                     eni, deo, input, p, output)
c************************************************************************
c
c.....Calls normcall for an array if inputs and return array of outputs
c
      implicit none
c.....variables
      integer n, u, i, o, conec(n,2), inno(i), outno(o), p
      double precision x(n), units(u), input(p,i), output(p,o)
      double precision eni(i,2), deo(o,2)
c.....helper variables
      double precision tmpinp(i), tmpout(o)
      integer j, k
c.....f2py statements
cf2py intent(out) output, istat

c.....iterate over input set
      do j=1,p
          do k=1,i
              tmpinp(k) = input(j,k)
          enddo
          call normcall(x, conec, n, units, u, inno, i, outno, o,
     &                  eni, deo, tmpinp, tmpout)
          do k=1,o
              output(j,k) = tmpout(k)
          enddo
      enddo
      
      return
      end
c
c************************************************************************
      subroutine normdiff2(x, conec, n, dconecno, dn, dconecmk, units,u, 
     &                     inno, i, outno, o, eni, ded, input, p, deriv)
c************************************************************************
c
c.....Calls normdiff for an array if inputs and return array of derivs
c
      implicit none
c.....variables
      integer n, dn, u, i, o, p, conec(n,2), dconecno(dn), dconecmk(i+1)
      integer inno(i), outno(o)
      double precision x(n), units(u), input(p,i), deriv(p,o,i)
      double precision eni(i,2), ded(o,i)
c.....helper variables
      integer j, k, l
      double precision tmpinp(i), tmpder(o,i)
c.....f2py statements
cf2py intent(out) deriv

c.....iterate over input set
      do j=1,p
          do k=1,i
              tmpinp(k) = input(j,k)
          enddo
          call normdiff(x, conec, n, dconecno, dn, dconecmk, units,
     &                  u, inno, i, outno, o, eni, ded, tmpinp, tmpder)
          do k=1,o
              do l=1,i
                  deriv(j,k,l) = tmpder(k,l)
              enddo
          enddo
      enddo
      
      return
      end
c
cc
ccc
cccc
ccccc BASIC TRAINING ALGORITHMS
cccc
ccc
cc
c
c************************************************************************
      subroutine momentum(x, conec, n, bconecno, bn, units, u, inno, i,  
     &                   outno, o, Input, Targ, p, eta, moment, maxiter)
c************************************************************************
c
c.....Standard backpropagation training with momentum
c
      implicit none
c.....variables
      integer n, bn, u, i, o, p, maxiter
      integer conec(n,2), bconecno(bn), inno(i), outno(o)
      double precision x(n), units(u), Input(p,i), Targ(p,o)
      double precision xprime(n), update, update0(n), eta, moment
c.....helper variables
      integer j, k
c.....f2py statements
cf2py intent(in, out) x
      
c.....initialize variables
      do j=1,n
          update0(j) = 0d0
      enddo
      k=0
c.....update maxiter times
      do while (k.lt.maxiter)
          call grad(x, conec, n, bconecno, bn, units, u, inno, i,  
     &              outno, o, Input, Targ, p, xprime)
	      do j=1,n
              update = -eta*xprime(j)
              x(j) = x(j) + update + moment*update0(j)
              update0(j) = update
          enddo
          k=k+1
      enddo
	  
      return
      end
      
c************************************************************************
      subroutine rprop(x, conec, n, bconecno, bn, units, u, inno, i,  
     &                 outno, o, Input, Targ, p, 
     &                 a, b, mimin, mimax, xmi, maxiter)
c************************************************************************
c
c.....Rprop training algorithm
c
      implicit none
c.....variables
      integer n, bn, u, i, o, p, maxiter
      integer conec(n,2), bconecno(bn), inno(i), outno(o)
      double precision x(n), units(u), Input(p,i), Targ(p,o)
      double precision xprime(n), xprime0(n), xmi(n)
      double precision a, b, mimax, mimin
c.....helper variables
      integer j, k
c.....f2py statements
cf2py intent(in, out) x, xmi
      
c.....initialize variables
      do j=1,n
          xprime0(j) = 0d0
      enddo
      k=0
c.....update maxiter times
      do while (k.lt.maxiter)
          call grad(x, conec, n, bconecno, bn, units, u, inno, i,  
     &              outno, o, Input, Targ, p, xprime)
	      do j=1,n
c.............find mi coefficient
              if ( xprime(j) * xprime0(j) .gt. 0 ) then
                  xmi(j) = min( a * xmi(j), mimax )
              elseif ( xprime(j) * xprime0(j) .lt. 0 ) then
                  xmi(j) = max( b * xmi(j), mimin )
              else
                  xmi(j) = xmi(j)
              endif
c.............update weights and record gradient components
              x(j) = x(j) - sign( xmi(j), xprime(j) )
              xprime0(j) = xprime(j)
          enddo
          k=k+1
      enddo
	  
      return
      end
c
cc
ccc
cccc
ccccc HELPER FUNCTIONS AND ROUTINES
cccc
ccc
cc
c
c************************************************************************
      subroutine setin(input, inno, i, eni, units, u)
c************************************************************************
c
c.....normalize and set input units
c
      implicit none
c.....variables
      integer i, u, inno(i)
      double precision input(i), units(u), eni(i,2)
c.....helper variables
      integer k
c.....f2py statements
cf2py intent(in,out) units

      do k=1,i
          units(inno(k)) = eni(k,1) * input(k) + eni(k,2)
      enddo

      return
      end

c************************************************************************
      subroutine getout(units, u, outno, o, deo, output)
c************************************************************************
c
c.....get and denormalize output units
c
      implicit none
c.....variables
      integer o, u, outno(o)
      double precision output(o), units(u), deo(o,2)
c.....helper variables
      integer k
c.....f2py statements
cf2py intent(out) output

      do k=1,o
          output(k) = deo(k,1) * units(outno(k)) + deo(k,2)
      enddo

      return
      end

c************************************************************************
      function mapa(f, a, b, c, d)
c************************************************************************
c
c.....Linear map of f from range (a,b) to range (c,d)
c
      implicit none
c.....variables
      double precision a, b, c, d, f, mapa
c.....helper variables
      double precision t    
      
c.....map vector (no check of bounds...)      
      t = ( d - c ) / ( b - a )
      mapa = c + ( f - a ) * t

      RETURN
      end

c************************************************************************
      function dmapa(f, a, b, c, d)
c************************************************************************
c
c.....Derivative of linear map of f from range (a,b) to range (c,d)
c.....(silly, but made for some generality purposes...)
c
      implicit none
c.....variables
      double precision a, b, c, d, f, dmapa
      
c.....map vector (no check of bounds...)      
      dmapa = ( d - c ) / ( b - a )

      RETURN
      end

c************************************************************************
      subroutine vmapa(vin, n, a, b, c, d, vout)
c************************************************************************
c
c.....Linear map of vector elements from range (a,b) to range (c,d)
c
      implicit none
c.....variables
      INTEGER n
      double precision a, b, c, d, vin(n), vout(n)
c.....helper variables
      integer k
      double precision t
c.....f2py statements
cf2py intent(out) vout      
      
c.....map vector (no check of bounds...)      
      t = ( d - c ) / ( b - a )
      do k=1,n   
          vout(k) = c + ( vin(k) - a ) * t
      enddo

      RETURN
      end

c************************************************************************
      subroutine mmapa(mmin, m, n, a, b, c, d, mmout)
c************************************************************************
c
c.....Linear map of matrix elements from range (a,b) to range (c,d)
c
      implicit none
c.....variables
      INTEGER m,n
      double precision a, b, c, d, mmin(m, n), mmout(m,n)
c.....helper variables
      integer j,k
      double precision t
c.....f2py statements
cf2py intent(out) mmout      
      
c.....map matrix (no check of bounds...)      
      t = ( d - c ) / ( b - a )
      do j=1,m
	      do k=1,n
              mmout(j,k) = c + ( mmin(j,k) - a ) * t
          enddo
      enddo

      RETURN
      end
	  
      