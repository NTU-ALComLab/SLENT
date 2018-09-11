package edu.nccu.soslab.regex2blif;

import java.io.IOException;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.MissingOptionException;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;

public class App
{
    private static Logger logger = Logger.getLogger(App.class);

    public static void main(String[] args)
    {
	BasicConfigurator.configure();
	
	Options options = new Options();
//	options.addOption(OptionBuilder.withLongOpt("integer-option").withDescription("description").withType(Number.class).hasArg().withArgName("argname").create());
	options.addOption(Option.builder("r").longOpt("regex").desc("regular expression").type(String.class).hasArg().required().build());
	options.addOption(Option.builder("b").longOpt("bits-per-char").desc("bits per character. Default: 8").type(Integer.class).hasArg().required(false).build());
	options.addOption(Option.builder("d").longOpt("dot").desc("output graphviz dotty format file. (Optional)").type(String.class).hasArg().required(false).build());
	options.addOption(Option.builder("o").longOpt("output").desc("output BLIF file").type(String.class).hasArg().required().build());
	options.addOption(Option.builder("m").longOpt("model-name").desc("BLIF model name. Must set with -o option. Default: 'regex'").type(String.class).hasArg().required(false).build());
	
	options.addOption(Option.builder("l").longOpt("log-level").desc("Log level: DEBUG, INFO, WARN, ERROR, FATAL").type(String.class).hasArg().required(false).build());
		

	CommandLineParser cliParser = new DefaultParser();
	CommandLine cmdLine = null;

	try
	{
	    cmdLine = cliParser.parse(options, args);
	    Logger.getRootLogger().setLevel(Level.toLevel(cmdLine.getOptionValue('l', "DEBUG")));
	    
	    int bitsPerChar = 8;
	    if(cmdLine.hasOption('b'))
	    {
		bitsPerChar = ((Integer)cmdLine.getParsedOptionValue("bits-per-char")).intValue();
	    }
	    Converter converter = new Converter(cmdLine.getOptionValue('r'), bitsPerChar);
	    
	    if(cmdLine.hasOption('d'))
	    {
		converter.exportDotty(cmdLine.getOptionValue('d'));
	    }
	    
	   
	    
	    if(cmdLine.hasOption('o'))
	    {
		 String blifModelName = cmdLine.getOptionValue('m', "regex");
		 
		 converter.exportBLIF(cmdLine.getOptionValue('o'), blifModelName);
	    }
	    
	}
	catch (ParseException e)
	{
	    printHelp(options);
	    logger.fatal(e);
	}
	catch (IOException e)
	{
	    logger.fatal(e);
	}
	
    }

    private static void printHelp(Options cliOptions)
    {
	HelpFormatter helpFormatter = new HelpFormatter();
	helpFormatter.printHelp("[options]", cliOptions);
    }
}
