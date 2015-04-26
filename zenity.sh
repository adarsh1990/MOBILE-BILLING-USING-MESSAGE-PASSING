#!/bin/sh
#script.sh:Shell script for Creating the front end for the project applcation  created by :MOHAMED FARAS,ADARSH SINGH,KIRAN UPADHYAYA,AJAY KUTINIKAR



tput clear

zenity  --info --text="\n\n\n\n\n\nMINI PROJECT APPLICATION\n\nTO IMPLEMENT MOBILE BILLING \nUSING MESSAGE PASSING\n\n\n\n\n\n\n"  --title="MOBILE BILLING USING MESSAGE PASSING" --height=200


while true


do

	choice=`zenity --list --radiolist --column "SELECT" --column "TYPE OF USER" FALSE "NEW USER" FALSE "OLD USER" TRUE "EXIT"`

	case $choice in
		"NEW USER")
				/home/mohamedfaras/Desktop/SUBMISSION/newuser

				;;

		"OLD USER")
				choice1=`zenity --list --radiolist --column "SELECT" --column "TYPE OF CUSTOMER" FALSE "PREPAID" FALSE "POSTPAID" `

				case $choice1 in

					PREPAID)
						choice2=`zenity --list --radiolist --column "SELECT" --column "TYPE OF OPERATIONS" FALSE "MAKE A CALL" FALSE "CHECK BALANCE" FALSE "RECHARGE"`

						case $choice2 in

							"MAKE A CALL")/home/mohamedfaras/Desktop/SUBMISSION/precall
									;;

							"CHECK BALANCE")/home/mohamedfaras/Desktop/SUBMISSION/balance
									;;

							"RECHARGE")/home/mohamedfaras/Desktop/SUBMISSION/recharge
									
						esac


						;;




					POSTPAID)

						choice3=`zenity --list --radiolist --column "SELECT" --column "TYPE OF OPERATIONS" FALSE "MAKE A CALL" FALSE "GENERATE BILL" FALSE "PAYMENT OF BILL"`

						case $choice3 in

							"MAKE A CALL")/home/mohamedfaras/Desktop/SUBMISSION/postcall
									;;

							"GENERATE BILL")/home/mohamedfaras/Desktop/SUBMISSION/billgen
									;;

							"PAYMENT OF BILL")/home/mohamedfaras/Desktop/SUBMISSION/billpay


						esac


				esac

					;;

			EXIT)zenity  --info --text="\n\n\n\nTECHNICAL GUIDE  :   Ms. MANIMALA M\n\n\n\nDEVELOPED BY  :\n\nKIRAN N UPADHYAYA\nADARSH SINGH M\nMOHAMED FARAS\nAJAY KUTINIKAR\n\n\n\nThank You For Using The Mini Application\n\n\n\n." --title="Credits" 
				break


	esac

done
