package edu.nccu.soslab.regex2blif;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import javax.sql.rowset.CachedRowSet;

import org.apache.commons.lang3.StringUtils;
import org.apache.log4j.Logger;

import dk.brics.automaton.Automaton;
import dk.brics.automaton.RegExp;
import dk.brics.automaton.State;
import dk.brics.automaton.Transition;

public class Converter
{
    private Logger logger = Logger.getLogger(Converter.class);
    private RegExp regex;
    private Automaton regexAuto;
    private int bitPerChar;

    public Converter(String regex, int bitsPerChar)
    {
	logger.debug("regex = " + regex);
	this.regex = new RegExp(regex);
	this.regexAuto = this.regex.toAutomaton();
	
	this.bitPerChar = bitsPerChar;
    }

    public String toDot()
    {
	String dotStr = regexAuto.toDot();
	logger.debug(dotStr);
	return dotStr;
    }

    public void exportDotty(String outputPath) throws IOException
    {
	try (FileWriter writer = new FileWriter(outputPath))
	{
	    writer.write(toDot());
	    writer.flush();
	}
    }

    public void exportBLIF(String outputPath, String modelName) throws IOException
    {
	try (FileWriter writer = new FileWriter(outputPath))
	{
	    PrintWriter printer = new PrintWriter(writer);

	    Map<State, Integer> stateNoMap = new HashMap<>(); // state node
							      // => state
							      // number in
							      // blif)

	    int noCounter = 0;
	    for (State state : regexAuto.getStates())
	    {
		stateNoMap.put(state, noCounter++);
	    }

	    /* calculate needed bits */
	    int inputBits = bitPerChar;
	    int stateBits = (int) Math.ceil(Math.log((double) regexAuto.getStates().size()) / Math.log(2f));

	    /* Write model name */
	    printer.println(".model " + modelName);

	    /* Write inputs */
	    printer.print(".inputs");
	    for (int i = 0; i < inputBits; i++)
	    {
		printer.print(" i" + (i + 1));
	    }
	    for (int i = 0; i < stateBits; i++)
	    {
		printer.print(" s" + (i + 1));
	    }
	    for (int i = 0; i < stateBits; i++)
	    {
		printer.print(" n" + (i + 1));
	    }
	    printer.print("\n");

	    /* Write outputs */
	    printer.println(".outputs i o t");

	    /* Write initial state */
	    printer.print(".names");

	    for (int i = 0; i < stateBits; i++)
	    {
		printer.print(" s" + (i + 1));
	    }
	    printer.println(" i");
	    printer.printf("%s 1\n\n", toBinaryString(stateNoMap.get(regexAuto.getInitialState()), stateBits, '0'));

	    /* write accepted state */
	    printer.print(".names");
	    for (int i = 0; i < stateBits; i++)
	    {
		printer.print(" s" + (i + 1));
	    }
	    printer.println(" o");
	    for (State acceptedState : regexAuto.getAcceptStates())
	    {
		int stateNo = stateNoMap.get(acceptedState);
		printer.printf("%s 1\n", toBinaryString(stateNo, stateBits, '0'));
	    }
	    printer.print("\n");

	    /* Write transition */
	    printer.print(".names");
	    for (int i = 0; i < inputBits; i++)
	    {
		printer.print(" i" + (i + 1));
	    }
	    for (int i = 0; i < stateBits; i++)
	    {
		printer.print(" s" + (i + 1));
	    }
	    for (int i = 0; i < stateBits; i++)
	    {
		printer.print(" n" + (i + 1));
	    }
	    printer.println(" t");

	    
	    char max = (char) (Math.pow(2, bitPerChar) - 1);

	    for (State srcState : regexAuto.getStates())
	    {
		int fromStateNo = stateNoMap.get(srcState);
		for (Transition transition : srcState.getTransitions())
		{
		    int toStateNo = stateNoMap.get(transition.getDest());

		    char lower = transition.getMin();
		    char upper = transition.getMax();

		    if (lower > max)
		    {
			logger.warn("transition will be removed because it character range is out of the bound: \n" + transition.toString());

			continue;
		    }

		    printer.printf("# %d ---( %d~%d )---> %d\n", fromStateNo,(int)lower,(int)upper ,toStateNo);

		    if (upper > max)
		    {
			upper = max;
			logger.warn("transition's character range will be truncated to " + lower + " ~ " + upper + " because its origin character range is out of the bound:\n "
				+ transition.toString());

		    }

		   
		    int lowerPlusOne = lower + 1;
		    int upperPlusOne = upper + 1;
		    double log2LowerPlusOne = Math.log((double) lowerPlusOne) / Math.log(2f);
		    double log2UpperPlusOne = Math.log((double) upperPlusOne) / Math.log(2f);
		    int lowerLevel = (int) Math.ceil(log2LowerPlusOne);
		    int upperLevelExcluded = (int) Math.floor(log2UpperPlusOne);
		    

		    for (int i = lower; i < (int)Math.min(Math.pow(2, lowerLevel), lower) ; i++)
		    {
			printer.print(toBinaryString(i, bitPerChar, '0'));
			printer.print(toBinaryString(fromStateNo, stateBits, '0'));
			printer.print(toBinaryString(toStateNo, stateBits, '0'));
			printer.println(" 1\n");

		    }
		    
		    for(int i = lowerLevel ; i< upperLevelExcluded ; i++)
		    {
			printer.print(String.join("", Collections.nCopies(bitPerChar - (i + 1), "0")) + "1" + String.join("", Collections.nCopies(i, "-")));
			printer.print(toBinaryString(fromStateNo, stateBits, '0'));
			printer.print(toBinaryString(toStateNo, stateBits, '0'));
			printer.println(" 1\n");
		    }
		    
		   

		    for (int i = (int)Math.max(Math.pow(2, upperLevelExcluded), upper) ; i < upper +1 ; i++)
		    {
			printer.print(toBinaryString(i, bitPerChar, '0'));
			printer.print(toBinaryString(fromStateNo, stateBits, '0'));
			printer.print(toBinaryString(toStateNo, stateBits, '0'));
			printer.println(" 1\n");
		    }

		}
	    }
	    printer.println(".end");
	    printer.flush();

	    writer.flush();
	}
    }

    public RegExp getRegex()
    {
	return regex;
    }

    public Automaton getRegexAuto()
    {
	return regexAuto;
    }

    private String toBinaryString(int number, int length, char padChar)
    {
	return StringUtils.leftPad(Integer.toBinaryString(number), length, padChar);
    }

    

}
