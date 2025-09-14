#!/bin/bash

FILE="lab_2_1.txt" #ชื่อไฟล์ = ไว้แบบนี้ได้
menu=1

while [ $menu -ne 5 ] 
  do 
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
                sed -n "/^id: $id\$/,/^$/p" "$FILE" || echo "ID not found."
            fi
            ;;
        2)
            echo -e "\n" >> "$FILE"
            read -p "Enter ID: " id
            echo "id: $id">> "$FILE"

            read -p "Enter Name: " name
            echo "name: $name" >> "$FILE"

            read -p "Enter Birth Date: " birth
            echo "birth date: $birth" >> "$FILE"

            read -p "Enter Address: " address
            echo "address: $address" >> "$FILE"
            
            echo "Contact created."
            grep "^$id:" "$FILE"
            ;;
        3)
            read -p "Enter ID to update: " id

            # ตรวจว่ามี ID ในไฟล์หรือไม่
            if grep -q "^id: $id$" "$FILE"; then
                read -p "Enter new name: " new_name
                
                # แก้ไขชื่อเฉพาะ block ของ ID นั้น
                sed -i "/^id: $id$/,/^$/s/^name: .*/name: $new_name/" "$FILE"
                
                echo "Name updated."
            else
                echo "ID not found."
            fi
            ;;
        4)
            read -p "Enter ID to delete: " id
            if grep -q "^id: $id$" "$FILE"; then
               sed -i "/^id: $id\$/,/^$/d" "$FILE"
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