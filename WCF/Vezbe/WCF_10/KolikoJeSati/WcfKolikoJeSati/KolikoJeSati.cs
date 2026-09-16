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
    }
}
