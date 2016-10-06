using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class ConsoleMode
    {
        private static Dictionary<String, String> ReadConfigFile(string path, StreamWriter logger)
        {
            Dictionary<String, String> parameters = new Dictionary<string, string>();
            using (StreamReader reader = new StreamReader(path))
            {
                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();
                    logger.WriteLine(line);
                    string[] words = line.Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                    parameters[words[0]] = words[1];
                }
            }

            return parameters;
        }

        private static void SaveController(Controller controller, string filename)
        {
            FileStream fileStream = File.Create(filename);
            using (StreamWriter writer = new StreamWriter(fileStream))
            {
                writer.Write(controller.ToString());
            }
        }

        private static Controller GeneticAlgorithm(int gaMode, int generations, int popSize, double crossProb, double mutProb, int tournSize)
        {
            Func<Simulation, Controller, double> evaluator = FitnessFuncs.AvoidCollisions;
            Population pop = new Population(popSize, gaMode);
            for (int i = 0; i < generations; i++)
            {
                pop.Evaluate(evaluator, 80, 7);
                using (System.IO.StreamWriter file = new System.IO.StreamWriter("log.txt", true))
                {
                    file.WriteLine(String.Format("{0}; {1:0.0000}; {2:0.0000}; {3:0.0000}; {4:0.0000}; " +
                     "{5:0.0000}; {6:0.0000}; {7:0.0000}; {8:0.0000}", i,
                     pop.Best.Fitness, pop.Best.SpeedFactor, pop.Best.MovementFactor, pop.Best.ProximityFactor,
                     pop.AvgFitness, pop.AvgSpeedFactor, pop.AvgMovementFactor, pop.AvgProximityFactor));
                }
                    Console.WriteLine(String.Format("{0}; {1:0.0000}; {2:0.0000}; {3:0.0000}; {4:0.0000}; " +
                     "{5:0.0000}; {6:0.0000}; {7:0.0000}; {8:0.0000}", i,
                     pop.Best.Fitness, pop.Best.SpeedFactor, pop.Best.MovementFactor, pop.Best.ProximityFactor,
                     pop.AvgFitness, pop.AvgSpeedFactor, pop.AvgMovementFactor, pop.AvgProximityFactor));
                //pop.RouletteWheelSelect();
                pop = pop.Select(tournSize);
                pop.Crossover(crossProb);
                pop.Mutate(mutProb);
            }
            pop.Evaluate(evaluator, 80, 7);
            return pop.Best;
        }

        public static void Main(String[] args)
        {
            Dictionary<String, String> parameters;
            if (args.Length != 1)
                return;

            DateTime currentDate = DateTime.Now;
            using (System.IO.StreamWriter file = new System.IO.StreamWriter("log.txt", true))
            {
                file.WriteLine("-------------------------------------------");
                file.WriteLine("---- NEW EXPERIMENT");
                file.WriteLine("-------------------------------------------");
                file.WriteLine(currentDate.ToString("dd/MM/yyyy HH:mm:ss"));
                file.WriteLine();
                file.WriteLine("Configuration parameters:");
                file.WriteLine();
                parameters = ReadConfigFile(args[0], file);
                file.WriteLine();
            }

            Simulation.LoadDefaultState(parameters["input"], false);
            Simulation.SetControlledRobot(int.Parse(parameters["robotId"]));
                

            if (parameters["algorithm"] == "GA")
            {
                int generations = 50;
                int popSize = 80;
                double crossProb = 0.75;
                double mutProb = 0.1;
                int tournSize = 3;
                int gaMode = parameters["gaMode"] == "hybrid" ? Population.GA_HYBRID : Population.GA_CONST_START;
                int.TryParse(parameters["generations"], out generations);
                int.TryParse(parameters["populationSize"], out popSize);
                double.TryParse(parameters["crossoverProb"], out crossProb);
                double.TryParse(parameters["mutationProb"], out mutProb);
                int.TryParse(parameters["tournamentSize"], out tournSize);
                var best = GeneticAlgorithm(gaMode, generations, popSize, crossProb, mutProb, tournSize);
                SaveController(best, parameters["outputDir"] + "\\" + currentDate.ToString("dd_MM_yyyy__HH_mm") + ".rcs");
            }
        }
    }
}
