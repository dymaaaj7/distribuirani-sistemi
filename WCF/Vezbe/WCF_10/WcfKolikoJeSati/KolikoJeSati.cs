namespace WCFTacnoVreme
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)]
    public class KolikoJeSati : IKolikoJeSati
    {
        DateTime pristupio = DateTime.Now;

        public PristupLog KadJeBioZadnjiPristup()
        {
            return new PristupLog()
            {
                HostName = Environment.MachineName,
                Timestamp = pristupio,
            };
        }

        public DateTime VratiVreme()
        {
            pristupio = DateTime.Now;
            return DateTime.Now;
        }

        public decimal ObaviOperaciju(decimal a, decimal b, string op)
        {
            switch (op)
            {
                case "+":
                    return op1 + op2;
                case "-":
                    return op1 - op2;
                case "*":
                    return op1 * op2;
                case "/":
                    if (op2 == 0)
                        throw new Exception("Deljenje nulom");
                    else
                        return op1 / op2;
                default:
                    throw new Exception("Nepoznata operacija.");
            }
        }
    }
}