


unsigned int Function::breakoutIterator{1};

Complex Function::getValue ()
{
	return this->value;
}

unsigned int Function::getFunctionIterator ()
{
	return this->functionIterator;
}

unsigned int Function::getBreakoutIterator ()
{
	return breakoutIterator;
}

Complex Function::getInput () 
{
	return this->input;
}

unsigned int Function::getIterations ()
{
	return this->iterations;
}

void Function::setValue (Complex value)
{
	this->value = value;
}

void Function::setFunctionIterator (unsigned int F_I)
{
	this->functionIterator = F_I;
}

void Function::setBreakoutIterator (unsigned int M_I)
{
	breakoutIterator = M_I;
}

void Function::setInput (Complex input)
{
	this->input = input;
}

void Function::setIterations (unsigned int iter)
{
	this->iterations = iter;
}


void Function::evaluate () {}
void Function::validateInput () {}

Complex Function::returnComplex()
{	
	this->evaluate();
	return this->value;
}



