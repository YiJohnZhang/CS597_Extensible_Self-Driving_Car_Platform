


/**
 *	@brief
 *	
 *	@param
 *	@return
 */
float calculate_ultrasonic_distance()
{

}

/**
 *	@brief
 *	
 *	@param
 *	@return
 */
float poll_ultrasonic_distance_handler(int trigger_pin, int echo_pin)
{
	float ultrasonic_distance = 0;
	if (trigger_pin == echo_pin)
	{
		mapped_pin = [trigger_pin]
		GPIOA_ModeCfg(GPIO_Pin_15, );
			// selects "A15"
			// todo: a mapping funtion so that I can do
			// digital 

	}


	// save to RBQ
	return ultrasonic_distance;
}

/**
 *	@brief
 *	
 *	@param
 *	@return
 */
void save_ultrasonic_distance_reading(int channel, RingBufferQueue &ultrasonic_rbq_obj)
{

	// save to RBQ
}


/**
 *	@brief
 *	
 *	@param
 *	@return
 */
float return_ultrasonic_distance(int channel)
{

}