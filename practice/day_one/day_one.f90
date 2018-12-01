module day_one
    implicit none  

    contains

    function calc (length, list) result (sum)
        implicit none 

        integer length, x, sum, curr, last
        integer, dimension(length) :: list

        last = list(length)            
        sum = 0

        do x = 1 , size(list), 1
            curr = list(x)
            if(last .EQ. curr) sum = sum + curr
            last = curr
        enddo
    end function calc

endmodule day_one

program one
    use day_one

    implicit none
    
    print*, calc(8, (/9,1,2,1,2,1,2,9/))

endprogram one