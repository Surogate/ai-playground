using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;

namespace Client2D.Utils
{
    class RelayCommand : ICommand
    {
        readonly Action<object>     execute_;
        readonly Predicate<object>  canExecute_;

        public RelayCommand(Action<object> execute)
        {
            execute_ = execute;
        }

        public RelayCommand(Action<object> execute, Predicate<object> canExecute)
        {
            if (execute == null)
                throw new ArgumentNullException("execute");
            execute_ = execute;
            canExecute_ = canExecute;
        }

        public bool CanExecute(object parameter)
        {
            return (canExecute_ == null) ? true : canExecute_(parameter);
        }

        public event EventHandler CanExecuteChanged;

        public void Execute(object parameter)
        {
            execute_(parameter);
        }
    }
}
