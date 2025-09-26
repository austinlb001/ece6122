# AUTHOR: AUSTIN LOGAN BARBER
# FILE NAME: make_background.py
# DATE CREATED: 2025 SEPTEMBER 18
# DATE LAST UPDATED: 2025 SEPTEMBER 18
# PURPOSE: This file creates the back ground image using the same color that is 
#     used in the "Start_Screen.png" file. 

################################# I M P O R T ##################################
import cv2
import numpy as np

################################################################################
################################################################################


################################### M A I N ####################################

if __name__ == "__main__":

    image = cv2.imread("../graphics/Start_Screen.png")
    print(image.shape)

    (B, G, R) = image[0][0]

    print("R = {}, G = {}, B = {}".format(R, G, B))

    background = np.zeros(image.shape, np.uint8)
    for i in range(0, background.shape[0]):
        for j in range(0, background.shape[1]):
            for k in range(0, background.shape[2]):
                
                if k == 0:
                    background[i][j][k] = B
                elif k == 1:
                    background[i][j][k] = G
                elif k == 2:
                    background[i][j][k] = R
                else: 
                    print("Error! Out of index!")

    cv2.imwrite("../graphics/background.png", background)
                

    



################################################################################
################################################################################
