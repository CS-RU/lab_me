#!/bin/bash

FILE="lab_2_2.txt" #ชื่อไฟล์ = ไว้แบบนี้ได้


while true; do #ใส่ให้เป็น true ไว้แบบนี้ก็ได้
   echo "1. Get contact by Id or get all"
   echo "2. Create contact"
   echo "3. Update contact name by Id"
   echo "4. Delete contact by Id"
   echo "5. Exit"

   echo -n "Enter menu: "
   read menu
   echo -e " \n" #-e \n เพื่อขึ้นบรรทัดใหม่

    case $menu in
        1)
            read -p "Enter ID (leave blank to get all): " id
            if [[ -z "$id" ]]; then
                cat "$FILE"
            else
                grep "^$id|" "$FILE" || echo "ID not found."
            fi
            ;;
        2)
            echo "" >> "$FILE"
            read -p "Enter ID: " id
            read -p "Enter Name: " name
            read -p "Enter Birth Date: " birth
            read -p "Enter Address: " address
            
            echo "$id|$name|$birth|$address" >> "$FILE"
            echo "Contact created."
            grep "^$id|" "$FILE"
            ;;
        3)
            read -p "Enter ID to update: " id
            if sed -n "/^id: $target_id\$/,/^$/p" "$FILE"; then
                read -p "Enter new name: " new_name
                
                sed -i "s/^\($id|\)[^|]*/\1$new_name/" "$FILE"
                echo "Name updated."
            else
                echo "ID not found."
            fi
            ;;
        4)
            read -p "Enter ID to delete: " id
            if grep -q "^$id|" "$FILE"; then
                sed -i "/^$id|/d" "$FILE"
                echo "Contact deleted."
            else
                echo "ID not found."
            fi
            ;;
        5)
            echo "Goodbye!"
            break
            ;;
        *)
            echo "Invalid pleass enter 1-5."
            ;;
    esac
    echo
done