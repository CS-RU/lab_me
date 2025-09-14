#!/bin/bash
FILE="lab_2_3.txt"

while true; do
    echo -e " \n"
    echo "1. Get book by ISBN or get all"
    echo "2. Add book to stock"
    echo "3. Remove book from stock"
    echo "4. Update book’s name by ISBN"
    echo "5. Delete book by ISBN"
    echo "6. Exit"
    read -p "Choose an option: " choice

    case $choice in
        1)
            read -p "Enter ISBN (leave empty for all): " isbn
            if [[ -z "$isbn" ]]; then
                cat "$FILE"
            else
                grep "^$isbn|" "$FILE" || echo "Book not found."
            fi
            ;;
        2)
            echo "" >> "$FILE" #ให้ขึ้นบรรทัดใหม่ก่อน

            read -p "Enter ISBN: " isbn
            if grep -q "^$isbn|" "$FILE"; then
                echo "Book already exists."
            else
                read -p "Enter Title: " title
                read -p "Enter Stock: " stock
                echo "$isbn|$title|$stock" >> "$FILE"
                echo "Book added."
            fi
            ;;
        3)
            read -p "Enter ISBN: " isbn
            if grep -q "^$isbn|" "$FILE"; then #ถ้า id มีจริงจะลบหนังสือใน stock
                read -p "Enter stock to remove: " remove_stock #รับจำนวน stock ที่จะลบ

                current_stock=$(grep "^$isbn|" "$FILE" | cut -d'|' -f3) #เช็คว่าตอนนี้ใน stock มีนส.กี่เล่ม
                new_stock=$((current_stock - remove_stock)) #จน. stock ที่เหลือ = ตอนนี้มีเท่าไหร่ - จำนวน stock ที่จะลบ
            
                sed -i "s/^\($isbn|[^|]*|\)[0-9]\+/\1$new_stock/" "$FILE"
                echo "Stock updated."
              
            else #ถ้า id ไม่มีจริง
                echo "Book not found."
            fi
            ;;
        4)
            read -p "Enter ISBN to update: " isbn
            if grep -q "^$isbn|" "$FILE"; then
                read -p "Enter new title: " new_title
                sed -i "s/^\($isbn|\)[^|]*/\1$new_title/" "$FILE"
                echo "Title updated."
            else
                echo "Book not found."
            fi
            ;;
        5)
            read -p "Enter ISBN to delete: " isbn
            if grep -q "^$isbn|" "$FILE"; then
                sed -i "/^$isbn|/d" "$FILE"
                echo "Book deleted."
            else
                echo "Book not found."
            fi
            ;;
        6)
            echo "Goodbye!"
            break
            ;;
        *)
            echo "Invalid choice."
            ;;
    esac
done