module advent 
    implicit none

    contains

    ! function unique(length, arr) result(uni)
    !     integer::length,i,e
    !     character,dimension(length)::arr
    !     character,dimension(length)::uni
    !     character::curr,next
    !     logical::found

    !     do i=1,length
    !         curr=arr(i)
    !         found=.FALSE.

    !         do e=1,length
    !             next=uni(e)
    !             if(curr.EQ.next)found=.TRUE.
    !         end do

    !         if(.NOT.found)uni(i)=curr
    !     end do
    ! end function unique

    function reverse_str(length,str) result(final)
        integer::i,length
        character(length)::str,final
        character::tmp
        do i = 1, length/2
            tmp = str(i:i)
            str(i:i) = str(length+1-i:length+1-i)
            str(length+1-i:length+1-i) = tmp
        end do
        final=str
    end function reverse_str

    function one() result(checksum)
        integer::checksum,two_count,three_count,count
        character(26),dimension(250)::input
        character(26)::curr
        character::letter,compare
        integer::i,e,x ! iterators
        logical::has_two,has_three
        
        two_count=0
        three_count=0

        open(1,file="input.txt",status="old")

        do i=1,size(input)
            read(1,*) input(i)
        end do

        close(1)

        do i=1,size(input)
            has_two=.FALSE.
            has_three=.FALSE.
            curr=input(i)

            do e=1,len(curr)
                letter=curr(e:e)
                count=0

                do x=1,len(curr)
                    compare=curr(x:x)
                    if(compare.EQ.letter)count=count+1
                end do

                if(count.EQ.2)has_two=.TRUE.
                if(count.EQ.3)has_three=.TRUE.
            end do

            if(has_two)two_count=two_count+1
            if(has_three)three_count=three_count+1
        end do

        checksum=two_count*three_count
    end function one

    function two() result(chars)
        character(26),dimension(250)::input
        character(26)::chars,matched,line_a,line_b
        character::letter_a,letter_b
        integer::wrong_count
        integer::i,e,x ! iterators

        open(2,file="input.txt",status="old")

        do i=1,size(input) 
            read(2,*)input(i)
        end do

        close(2)

        do i=1,size(input)/2
            do e=1,size(input)
                matched=""
                wrong_count=0
                line_a=input(i)
                line_b=input(e)

                do x=1,len(line_a)
                    letter_a=line_a(x:x)
                    letter_b=line_b(x:x)

                    if(letter_a.EQ.letter_b)then
                        matched=letter_a//matched
                    else
                        wrong_count=wrong_count+1
                    end if 
                end do 

                if(wrong_count.EQ.1)chars=reverse_str(len(matched),matched)
            end do
        end do 
    end function two
    
end module advent

program day
    use advent
    implicit none
    print*,one()
    print*,two()
end program day