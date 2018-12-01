module util
    implicit none
    contains
    function get_count(find, length, arr) result(count)
        integer::find,length,count,pos
        integer,dimension(length)::arr
        count = 0
        do pos=1,length
            if(find.EQ.arr(pos))count = count + 1
        end do
    end function get_count
end module util

module advent_of_code
    use util
    implicit none

    contains

    function part_one() result(sum) 
        integer, dimension(973) :: input
        integer :: i, sum

        sum = 0
        
        open (1, file = 'input.txt', status = 'old')
    
        do i = 1,size(input)  
        read(1,*) input(i)
        end do 
        
        close(1)
        
        do i = 1,size(input)  
            sum = sum + input(i)
        end do 
    end function part_one

    ! THIS IS SLOW AND BAD AND HORRIBLE
    function part_two() result(val)
        integer,dimension(999999)::all
        integer,dimension(973)::input
        integer::offset,sum,i,count,loop_count,val
        logical::found=.FALSE.

        loop_count = 0
        offset = 0

    1   open(2,file="input.txt",status="old")
        do i=1,size(input)
            read(2,*)input(i)
        end do
        close(2)

        do i=1,size(input) 
            sum=sum+input(i)
            all(i+offset)=sum
            count = get_count(sum,size(all),all)
            print*,sum," appears ",count," times ",loop_count
            if(count.EQ.2) then
                val=sum
                found=.TRUE.
            end if
        end do

        loop_count = loop_count + 1
        offset = offset + size(input)

        if(.NOT.found) goto 1
    end function part_two

end module advent_of_code

program day_one  
    use advent_of_code
    implicit none

    print*,part_one()
    print*,part_two()
    
end program day_one