/*****************************************************************************************
 * Project Name			: common_macros.h												 *
 * Description			: Common used macros										 	 *
 * Created on			: Jul 29, 2023													 *
 * Author				: Mazen Khaled													 *
 *****************************************************************************************
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

//For setting bit inside a register
#define SET_BIT(REG,BIT_NUM) (REG|=(1<< BIT_NUM))

//For Clearing bit inside a register
#define CLEAR_BIT(REG,BIT_NUM) (REG&=(~(1<< BIT_NUM)))

//For Toggling bit inside a register
#define TOGGLE_BIT(REG,BIT_NUM) (REG^=(1<< BIT_NUM))

/* Rotate right the register value with specific number of rotates */
#define ROR(REG,num)( REG = (REG>>num) | (REG<<(8-num)) )

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,num)( REG = (REG<<num) | (REG>>(8-num)) )

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT)( REG & (1<<BIT) )

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT)( !(REG & (1<<BIT)) )

#endif /* COMMON_MACROS_H_ */
